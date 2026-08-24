#include "roo/runtime/worker.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <vector>

#include <roo/exception.h>
#include <roo/runtime/value.h>

#include <gtest/gtest.h>

namespace
{
  void wait_until_terminal(const Roo::WorkerRegistry& registry,
                           const Roo::sptr_val& execution)
  {
    while (true)
    {
      const auto status = registry.poll(execution);
      if (status == Roo::WorkerExecutionStatus::SUCCEEDED ||
          status == Roo::WorkerExecutionStatus::FAILED)
      {
        return;
      }
      std::this_thread::yield();
    }
  }
} // namespace

TEST(WorkerRegistry, executes_jobs_in_fifo_order_and_reuses_worker_runtime)
{
  Roo::WorkerRegistry registry;
  registry.create("test/worker");

  std::mutex gate_mutex;
  std::condition_variable gate_changed;
  bool release_first = false;
  std::vector<int> order;

  Roo::sptr_val first =
    registry.enqueue("test/worker",
                     [&](Roo::Runtime&)
                     {
                       std::unique_lock lock(gate_mutex);
                       gate_changed.wait(lock, [&]() { return release_first; });
                       order.push_back(1);
                     });
  Roo::sptr_val second =
    registry.enqueue("test/worker", [&](Roo::Runtime&) { order.push_back(2); });

  while (registry.poll(first) == Roo::WorkerExecutionStatus::QUEUED)
  {
    std::this_thread::yield();
  }
  EXPECT_EQ(registry.poll(first), Roo::WorkerExecutionStatus::RUNNING);
  EXPECT_EQ(registry.poll(second), Roo::WorkerExecutionStatus::QUEUED);

  {
    std::lock_guard lock(gate_mutex);
    release_first = true;
  }
  gate_changed.notify_one();

  wait_until_terminal(registry, second);
  EXPECT_EQ(registry.poll(first), Roo::WorkerExecutionStatus::SUCCEEDED);
  EXPECT_EQ(registry.poll(second), Roo::WorkerExecutionStatus::SUCCEEDED);
  EXPECT_EQ(order, (std::vector<int>{1, 2}));
}

TEST(WorkerRegistry, failed_job_does_not_stop_later_jobs)
{
  Roo::WorkerRegistry registry;
  registry.create("test/worker");
  std::atomic<bool> later_job_ran = false;

  Roo::sptr_val failed =
    registry.enqueue("test/worker",
                     [](Roo::Runtime&) { throw std::runtime_error("expected failure"); });
  Roo::sptr_val later =
    registry.enqueue("test/worker", [&](Roo::Runtime&) { later_job_ran = true; });

  wait_until_terminal(registry, later);
  EXPECT_EQ(registry.poll(failed), Roo::WorkerExecutionStatus::FAILED);
  EXPECT_EQ(registry.poll(later), Roo::WorkerExecutionStatus::SUCCEEDED);
  EXPECT_TRUE(later_job_ran);
}

TEST(WorkerRegistry, execution_handle_routes_only_with_its_owning_registry)
{
  Roo::WorkerRegistry owner;
  Roo::WorkerRegistry other;
  owner.create("test/worker");
  other.create("test/worker");

  Roo::sptr_val execution = owner.enqueue("test/worker", [](Roo::Runtime&) {});

  EXPECT_THROW(other.poll(execution), Roo::RooException);
  EXPECT_NO_THROW(owner.poll(execution));
  EXPECT_EQ(execution->to_string(), "#<roo.worker/execution :test/worker 1>");
}

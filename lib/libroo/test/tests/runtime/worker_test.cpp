#include "roo/runtime/worker_internal.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <vector>

#include <roo/exception.h>
#include <roo/runtime/deep_copy.h>
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

  void wait_until_terminal(Roo::Worker& worker, uint64_t execution_id)
  {
    while (true)
    {
      const auto execution = worker.find_execution(execution_id);
      ASSERT_NE(execution, nullptr);
      const auto status = execution->status();
      if (status == Roo::WorkerExecutionStatus::SUCCEEDED ||
          status == Roo::WorkerExecutionStatus::FAILED)
      {
        return;
      }
      std::this_thread::yield();
    }
  }

  Roo::sptr_val invoke_identity(Roo::WorkerRegistry& registry,
                                const std::string& identity,
                                Roo::sptr_val value)
  {
    return registry.invoke(
      identity,
      Roo::Value::keyword("value"),
      {Roo::Value::map({Roo::Value::keyword("value"), std::move(value)})});
  }
} // namespace

TEST(WorkerRegistry, executes_jobs_in_fifo_order_and_reuses_worker_runtime)
{
  Roo::Worker worker;

  std::mutex gate_mutex;
  std::condition_variable gate_changed;
  bool release_first = false;
  std::vector<int> order;

  uint64_t first = worker.enqueue(
    [&](Roo::Runtime&)
    {
      std::unique_lock lock(gate_mutex);
      gate_changed.wait(lock, [&]() { return release_first; });
      order.push_back(1);
      return Roo::Constant::NIL;
    });
  uint64_t second = worker.enqueue(
    [&](Roo::Runtime&)
    {
      order.push_back(2);
      return Roo::Constant::NIL;
    });

  while (worker.find_execution(first)->status() == Roo::WorkerExecutionStatus::QUEUED)
  {
    std::this_thread::yield();
  }
  EXPECT_EQ(worker.find_execution(first)->status(), Roo::WorkerExecutionStatus::RUNNING);
  EXPECT_EQ(worker.find_execution(second)->status(), Roo::WorkerExecutionStatus::QUEUED);

  {
    std::lock_guard lock(gate_mutex);
    release_first = true;
  }
  gate_changed.notify_one();

  wait_until_terminal(worker, second);
  EXPECT_EQ(worker.find_execution(first)->status(), Roo::WorkerExecutionStatus::SUCCEEDED);
  EXPECT_EQ(worker.find_execution(second)->status(), Roo::WorkerExecutionStatus::SUCCEEDED);
  EXPECT_EQ(order, (std::vector<int>{1, 2}));
}

TEST(WorkerRegistry, failed_job_does_not_stop_later_jobs)
{
  Roo::Worker worker;
  std::atomic<bool> later_job_ran = false;

  uint64_t failed = worker.enqueue([](Roo::Runtime&) -> Roo::sptr_val
                                   { throw std::runtime_error("expected failure"); });
  uint64_t later = worker.enqueue(
    [&](Roo::Runtime&)
    {
      later_job_ran = true;
      return Roo::Constant::NIL;
    });

  wait_until_terminal(worker, later);
  EXPECT_EQ(worker.find_execution(failed)->status(), Roo::WorkerExecutionStatus::FAILED);
  EXPECT_EQ(worker.find_execution(later)->status(), Roo::WorkerExecutionStatus::SUCCEEDED);
  EXPECT_TRUE(later_job_ran);
  EXPECT_THROW(worker.collect(failed), std::runtime_error);
}

TEST(WorkerRegistry, roo_failure_is_reconstructed_from_its_transferred_error_map)
{
  Roo::Worker worker;
  uint64_t failed = worker.enqueue(
    [](Roo::Runtime&) -> Roo::sptr_val
    {
      Roo::TypeError error("expected Roo failure");
      error.set_form_site("worker-form", "worker-source.roo:4");
      throw error;
    });

  wait_until_terminal(worker, failed);
  try
  {
    worker.collect(failed);
    FAIL() << "Expected collection to raise the worker failure.";
  }
  catch (const Roo::RaisedError& error)
  {
    EXPECT_EQ(error.roo_error_type(), "roo/type-error");
    EXPECT_NE(error.to_error_map()->to_string().find("worker-source.roo:4"),
              std::string::npos);
  }
}

TEST(WorkerRegistry, execution_handle_routes_only_with_its_owning_registry)
{
  Roo::WorkerRegistry owner;
  Roo::WorkerRegistry other;
  owner.create("test/worker");
  other.create("test/worker");

  Roo::sptr_val execution = invoke_identity(owner, "test/worker", Roo::Constant::NIL);

  EXPECT_THROW(other.poll(execution), Roo::RooException);
  EXPECT_NO_THROW(owner.poll(execution));
  EXPECT_EQ(execution->to_string(), "#<roo.worker/execution :test/worker 1>");
}

TEST(WorkerRegistry, execution_handles_cannot_cross_runtime_boundaries)
{
  Roo::WorkerRegistry registry;
  registry.create("test/worker");
  Roo::sptr_val execution = invoke_identity(registry, "test/worker", Roo::Constant::NIL);

  EXPECT_THROW(Roo::deep_copy_for_runtime_transfer(execution), Roo::RooException);
}

TEST(WorkerRegistry, collect_returns_result_and_evicts_execution)
{
  Roo::WorkerRegistry registry;
  registry.create("test/worker");
  Roo::sptr_val execution =
    invoke_identity(registry, "test/worker", Roo::Value::vector({Roo::Value::number(7)}));

  wait_until_terminal(registry, execution);
  Roo::sptr_val result = registry.collect(execution);

  EXPECT_EQ(*result, *Roo::Value::vector({Roo::Value::number(7)}));
  EXPECT_THROW(registry.poll(execution), Roo::RooException);
  EXPECT_THROW(registry.collect(execution), Roo::RooException);
}

TEST(WorkerRegistry, collect_rejects_an_unfinished_execution)
{
  Roo::Worker worker;
  std::mutex gate_mutex;
  std::condition_variable gate_changed;
  bool release = false;
  uint64_t execution = worker.enqueue(
    [&](Roo::Runtime&)
    {
      std::unique_lock lock(gate_mutex);
      gate_changed.wait(lock, [&]() { return release; });
      return Roo::Constant::NIL;
    });

  EXPECT_THROW(worker.collect(execution), Roo::RooException);

  {
    std::lock_guard lock(gate_mutex);
    release = true;
  }
  gate_changed.notify_one();
}

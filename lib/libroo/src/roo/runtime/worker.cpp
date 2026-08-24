#include "worker.h"

#include <utility>

#include <roo/exception.h>
#include <roo/runtime.h>

namespace Roo
{
  Worker::Worker()
    : execution_thread(&Worker::run, this)
  {
    std::unique_lock lock(mutex);
    state_changed.wait(lock, [this]() { return ready; });
    if (startup_failure)
    {
      lock.unlock();
      execution_thread.join();
      std::rethrow_exception(startup_failure);
    }
  }

  Worker::~Worker()
  {
    {
      std::lock_guard lock(mutex);
      stopping = true;
    }
    state_changed.notify_one();
    execution_thread.join();
  }

  void Worker::run()
  {
    try
    {
      Runtime runtime;
      std::unique_lock lock(mutex);
      ready = true;
      state_changed.notify_one();
      state_changed.wait(lock, [this]() { return stopping; });
    }
    catch (...)
    {
      std::lock_guard lock(mutex);
      startup_failure = std::current_exception();
      ready = true;
      state_changed.notify_one();
    }
  }

  void WorkerRegistry::create(const std::string& identity)
  {
    if (workers.contains(identity))
    {
      throw RooException("Worker :" + identity + " already exists.");
    }

    workers.emplace(identity, std::make_unique<Worker>());
  }

  void Runtime::create_worker(const std::string& identity)
  {
    if (!workers)
    {
      workers = std::make_unique<WorkerRegistry>();
    }
    workers->create(identity);
  }
} // namespace Roo

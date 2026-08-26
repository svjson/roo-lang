#ifndef ROO__RUNTIME__WORKER_INTERNAL_H
#define ROO__RUNTIME__WORKER_INTERNAL_H

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <roo/runtime/worker.h>

namespace Roo
{
  class Runtime;

  class WorkerExecution
  {
   private:
    uint64_t execution_id;
    std::atomic<WorkerExecutionStatus> execution_status = WorkerExecutionStatus::QUEUED;
    sptr_val result;
    std::exception_ptr failure;

    void mark_running();
    void mark_succeeded(sptr_val value);
    void mark_failed(std::exception_ptr error);

    friend class Worker;

   public:
    explicit WorkerExecution(uint64_t execution_id);

    uint64_t id() const;
    WorkerExecutionStatus status() const;
    void rethrow_failure() const;
  };

  using WorkerTask = std::function<sptr_val(Runtime&)>;

  class Worker
  {
   private:
    struct QueuedExecution
    {
      std::shared_ptr<WorkerExecution> execution;
      WorkerTask task;
    };

    std::mutex mutex;
    std::condition_variable state_changed;
    std::thread execution_thread;
    std::deque<QueuedExecution> queue;
    std::map<uint64_t, std::shared_ptr<WorkerExecution>> executions;
    std::exception_ptr startup_failure;
    uint64_t next_execution_id = 1;
    bool ready = false;
    bool stopping = false;

    void run();

   public:
    Worker();
    ~Worker();

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;

    uint64_t enqueue(WorkerTask task);
    std::shared_ptr<const WorkerExecution> find_execution(uint64_t execution_id);
    sptr_val collect(uint64_t execution_id);
  };

} // namespace Roo

#endif

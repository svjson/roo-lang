#include <memory>
#include <utility>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/host/object.h>
#include <roo/host/type.h>
#include <roo/runtime.h>
#include <roo/runtime/deep_copy.h>

#include "runtime_transfer.h"
#include "worker_internal.h"

namespace Roo
{
  namespace
  {
    const HostTypeRef WORKER_EXECUTION_HANDLE_TYPE("roo.worker/execution");

    class WorkerExecutionHandleAdapter : public NativeObjectBase,
                                         public RuntimeLocalNativeObject
    {
     private:
      std::string worker_identity;
      uint64_t execution_id;
      std::weak_ptr<Worker> worker;
      sptr_val worker_value;
      sptr_val id_value;

      static sptr_val get_worker(const NativeObjectBase* value)
      {
        return static_cast<const WorkerExecutionHandleAdapter*>(value)->worker_value;
      }

      static sptr_val get_id(const NativeObjectBase* value)
      {
        return static_cast<const WorkerExecutionHandleAdapter*>(value)->id_value;
      }

     public:
      WorkerExecutionHandleAdapter(std::string worker_identity,
                                   uint64_t execution_id,
                                   const std::shared_ptr<Worker>& worker)
        : worker_identity(std::move(worker_identity))
        , execution_id(execution_id)
        , worker(worker)
        , worker_value(Value::keyword(this->worker_identity))
        , id_value(Value::number(static_cast<long long>(execution_id)))
      {
      }

      const NativeObjectTraits* get_traits() const override
      {
        static const NAccessorTable accessors{
          {"worker", NAccessors(get_worker, n_no_setter)},
          {"id", NAccessors(get_id, n_no_setter)}};
        static const NativeObjectTraits traits(&WORKER_EXECUTION_HANDLE_TYPE, accessors);
        return &traits;
      }

      std::string to_string() const override
      {
        return "#<roo.worker/execution :" + worker_identity + " " +
               std::to_string(execution_id) + ">";
      }

      bool equals_value(const Value& other) const override
      {
        if (other.type != Value::Type::NATIVE_OBJECT) return false;
        auto* other_handle = dynamic_cast<WorkerExecutionHandleAdapter*>(other.nobj().get());
        if (!other_handle) return false;

        return worker_identity == other_handle->worker_identity &&
               execution_id == other_handle->execution_id &&
               !worker.owner_before(other_handle->worker) &&
               !other_handle->worker.owner_before(worker);
      }

      void* self_object_ptr() const override
      {
        return const_cast<WorkerExecutionHandleAdapter*>(this);
      }

      const std::string& identity() const { return worker_identity; }
      uint64_t id() const { return execution_id; }
      std::shared_ptr<Worker> lock_worker() const { return worker.lock(); }
    };

    const WorkerExecutionHandleAdapter& execution_handle(const sptr_val& value)
    {
      if (!value || value->type != Value::Type::NATIVE_OBJECT)
      {
        throw RooException("Expected a worker execution handle.");
      }

      auto* handle = dynamic_cast<WorkerExecutionHandleAdapter*>(value->nobj().get());
      if (!handle)
      {
        throw RooException("Expected a worker execution handle.");
      }
      return *handle;
    }

  } // namespace

  struct WorkerRegistry::Impl
  {
    std::map<std::string, std::shared_ptr<Worker>> workers;
  };

  WorkerRegistry::WorkerRegistry()
    : impl(std::make_unique<Impl>())
  {
  }

  WorkerRegistry::~WorkerRegistry() = default;

  WorkerExecution::WorkerExecution(uint64_t execution_id)
    : execution_id(execution_id)
  {
  }

  uint64_t WorkerExecution::id() const
  {
    return execution_id;
  }

  WorkerExecutionStatus WorkerExecution::status() const
  {
    return execution_status.load(std::memory_order_acquire);
  }

  void WorkerExecution::rethrow_failure() const
  {
    if (status() != WorkerExecutionStatus::FAILED)
    {
      throw RooException("Worker execution has not failed.");
    }
    std::rethrow_exception(failure);
  }

  void WorkerExecution::mark_running()
  {
    execution_status.store(WorkerExecutionStatus::RUNNING, std::memory_order_release);
  }

  void WorkerExecution::mark_succeeded(sptr_val value)
  {
    result = std::move(value);
    execution_status.store(WorkerExecutionStatus::SUCCEEDED, std::memory_order_release);
  }

  void WorkerExecution::mark_failed(std::exception_ptr error)
  {
    failure = std::move(error);
    execution_status.store(WorkerExecutionStatus::FAILED, std::memory_order_release);
  }

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

      while (true)
      {
        state_changed.wait(lock, [this]() { return stopping || !queue.empty(); });
        if (stopping && queue.empty()) return;

        QueuedExecution queued = std::move(queue.front());
        queue.pop_front();
        queued.execution->mark_running();

        lock.unlock();
        try
        {
          sptr_val result = queued.task(runtime);
          queued.execution->mark_succeeded(deep_copy_for_runtime_transfer(result));
        }
        catch (...)
        {
          queued.execution->mark_failed(std::current_exception());
        }
        lock.lock();
        state_changed.notify_all();
      }
    }
    catch (...)
    {
      std::lock_guard lock(mutex);
      startup_failure = std::current_exception();
      ready = true;
      state_changed.notify_one();
    }
  }

  uint64_t Worker::enqueue(WorkerTask task)
  {
    std::lock_guard lock(mutex);
    if (stopping)
    {
      throw RooException("Cannot enqueue work on a stopping worker.");
    }

    const uint64_t execution_id = next_execution_id++;
    auto execution = std::make_shared<WorkerExecution>(execution_id);
    executions.emplace(execution_id, execution);
    queue.push_back({execution, std::move(task)});
    state_changed.notify_one();
    return execution_id;
  }

  std::shared_ptr<const WorkerExecution> Worker::find_execution(uint64_t execution_id)
  {
    std::lock_guard lock(mutex);
    auto found = executions.find(execution_id);
    if (found == executions.end()) return nullptr;
    return found->second;
  }

  sptr_val Worker::collect(uint64_t execution_id)
  {
    std::unique_lock lock(mutex);
    auto found = executions.find(execution_id);
    if (found == executions.end())
    {
      throw RooException("Worker execution does not exist.");
    }

    std::shared_ptr<WorkerExecution> execution = found->second;
    const WorkerExecutionStatus execution_status = execution->status();
    if (execution_status == WorkerExecutionStatus::QUEUED ||
        execution_status == WorkerExecutionStatus::RUNNING)
    {
      throw RooException("Worker execution has not finished.");
    }

    executions.erase(found);
    lock.unlock();

    if (execution_status == WorkerExecutionStatus::FAILED)
    {
      execution->rethrow_failure();
    }
    return std::move(execution->result);
  }

  void WorkerRegistry::create(const std::string& identity)
  {
    if (impl->workers.contains(identity))
    {
      throw RooException("Worker :" + identity + " already exists.");
    }

    impl->workers.emplace(identity, std::make_shared<Worker>());
  }

  sptr_val WorkerRegistry::enqueue(const std::string& identity, Task task)
  {
    auto found = impl->workers.find(identity);
    if (found == impl->workers.end())
    {
      throw RooException("Worker :" + identity + " does not exist.");
    }

    const uint64_t execution_id = found->second->enqueue(std::move(task));
    return Value::native_object(
      std::make_shared<WorkerExecutionHandleAdapter>(identity, execution_id, found->second));
  }

  sptr_val WorkerRegistry::invoke(const std::string& identity,
                                  const sptr_val& callable,
                                  const sptr_val_v& arguments)
  {
    sptr_val_v roots;
    roots.reserve(arguments.size() + 1);
    roots.push_back(callable);
    roots.insert(roots.end(), arguments.begin(), arguments.end());
    sptr_val_v transferred = deep_copy_for_runtime_transfer(roots);

    sptr_val transferred_callable = transferred.front();
    sptr_val_v transferred_arguments(transferred.begin() + 1, transferred.end());
    return enqueue(identity,
                   [callable = std::move(transferred_callable),
                    arguments = std::move(transferred_arguments)](Runtime& runtime) mutable
                   {
                     Context ctx(runtime);
                     return invoke_callable(ctx, callable, arguments);
                   });
  }

  WorkerExecutionStatus WorkerRegistry::poll(const sptr_val& execution_handle_value) const
  {
    const auto& handle = execution_handle(execution_handle_value);
    auto found = impl->workers.find(handle.identity());
    auto handle_worker = handle.lock_worker();
    if (found == impl->workers.end() || !handle_worker || found->second != handle_worker)
    {
      throw RooException("Worker execution handle does not belong to this runtime.");
    }

    auto execution = handle_worker->find_execution(handle.id());
    if (!execution)
    {
      throw RooException("Worker execution does not exist.");
    }
    return execution->status();
  }

  sptr_val WorkerRegistry::collect(const sptr_val& execution_handle_value)
  {
    const auto& handle = execution_handle(execution_handle_value);
    auto found = impl->workers.find(handle.identity());
    auto handle_worker = handle.lock_worker();
    if (found == impl->workers.end() || !handle_worker || found->second != handle_worker)
    {
      throw RooException("Worker execution handle does not belong to this runtime.");
    }
    return handle_worker->collect(handle.id());
  }

  WorkerRegistry& Runtime::worker_registry()
  {
    if (!workers)
    {
      workers = std::make_unique<WorkerRegistry>();
    }
    return *workers;
  }
} // namespace Roo

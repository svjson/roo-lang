#ifndef ROO__RUNTIME__WORKER_H
#define ROO__RUNTIME__WORKER_H

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include <roo/export.h>
#include <roo/runtime/value.h>

namespace Roo
{
  class Runtime;

  /**
   * @brief Own the Runtime and host resources used by one worker thread.
   * @since 0.1.0
   *
   * WorkerRegistry invokes environment factories and destroys their results on
   * the worker thread. Host implementations may therefore retain file systems,
   * namespace source backing stores, native-package handles, and other
   * resources that must outlive the Runtime without exposing them to the
   * worker controller.
   */
  class ROO_API WorkerEnvironment
  {
   public:
    virtual ~WorkerEnvironment() = default;

    /**
     * @brief Return the runtime owned by this environment.
     * @return Runtime confined to the worker's execution thread.
     */
    virtual Runtime& runtime() = 0;
  };

  using WorkerEnvironmentFactory = std::function<std::unique_ptr<WorkerEnvironment>()>;

  enum class WorkerExecutionStatus : uint8_t
  {
    QUEUED,
    RUNNING,
    SUCCEEDED,
    FAILED
  };

  /**
   * @brief Own and route the worker runtimes belonging to one parent Runtime.
   * @since 0.1.0
   *
   * Worker controllers, queues, and execution records remain private to the
   * implementation. Callable invocation is the public scheduling boundary and
   * applies the strict runtime-transfer policy to the callable and arguments.
   */
  class ROO_API WorkerRegistry
  {
   private:
    using Task = std::function<sptr_val(Runtime&)>;
    struct Impl;
    std::unique_ptr<Impl> impl;

    sptr_val enqueue(const std::string& identity, Task task);

   public:
    WorkerRegistry();
    ~WorkerRegistry();

    WorkerRegistry(const WorkerRegistry&) = delete;
    WorkerRegistry& operator=(const WorkerRegistry&) = delete;

    /**
     * @brief Register a named host-provided worker environment factory.
     * @param name Environment name selected during worker creation.
     * @param factory Factory invoked on each selected worker's own thread.
     */
    void register_environment(const std::string& name, WorkerEnvironmentFactory factory);

    /**
     * @brief Create and start a named vanilla worker runtime.
     * @param identity Worker identity, unique within this registry.
     */
    void create(const std::string& identity);

    /**
     * @brief Create and start a worker using a named environment factory.
     * @param identity Worker identity, unique within this registry.
     * @param environment Registered environment name, or `vanilla`.
     */
    void create(const std::string& identity, const std::string& environment);

    /**
     * @brief Transfer and asynchronously invoke a callable in a named worker.
     * @param identity Identity of an existing worker.
     * @param callable Callable transferred to and invoked in the worker.
     * @param arguments Argument roots transferred with the callable.
     * @return A runtime-local execution handle.
     */
    sptr_val invoke(const std::string& identity,
                    const sptr_val& callable,
                    const sptr_val_v& arguments);

    /**
     * @brief Read the current state of an execution without consuming it.
     * @param execution_handle Runtime-local worker execution handle.
     * @return Current queued, running, succeeded, or failed state.
     */
    WorkerExecutionStatus poll(const sptr_val& execution_handle) const;

    /**
     * @brief Consume a terminal execution's result or retained failure.
     * @param execution_handle Runtime-local worker execution handle.
     * @return Transported worker result.
     */
    sptr_val collect(const sptr_val& execution_handle);
  };
} // namespace Roo

#endif

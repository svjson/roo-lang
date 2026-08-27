#ifndef ROO__LANG__WORKER__WORKER_H
#define ROO__LANG__WORKER__WORKER_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Consume and return a completed worker execution's result.
   * @since 0.1.0
   *
   * Collection removes the execution and its retained result from the worker.
   * Collecting an unfinished, already collected, or unknown execution is an
   * error. A failed execution raises its failure and is likewise removed.
   *
   * Usage:
   * @code
   * (roo.worker/collect! execution)
   * => {:answer 42}
   * @endcode
   *
   * | Arg       | Description                                                |
   * | --------- | ---------------------------------------------------------- |
   * | execution | Completed execution handle returned by a worker operation. |
   *
   * @return The unwrapped worker execution result.
   */
  FUNC(CollectWorkerBangFunction, collect_worker)

  /*!
   * @brief Create and start a named worker owned by the current runtime.
   * @since 0.1.0
   *
   * The worker is registered under a runtime-local keyword identity and owns a
   * dedicated execution thread. The default vanilla environment has no
   * application namespaces, namespace loading, or file-system access inherited
   * from the parent. A host may register an application environment that
   * reconstructs its runtime bootstrap without invoking its entry point. The
   * optional `:autoloads` vector names additional worker-local namespaces to
   * load after environment initialization. The call returns only after worker
   * initialization and these autoloads succeed.
   *
   * Creating another worker with the same identity in the current runtime is an
   * error. The same identity may be used independently by another runtime.
   *
   * Usage:
   * @code
   * (roo.worker/create! :my-app/worker) => nil
   * (roo.worker/create! :my-app/application-worker
   *                     {:environment :application
   *                      :autoloads ["my-app.worker"]}) => nil
   * @endcode
   *
   * | Arg      | Description                                                  |
   * | -------- | ------------------------------------------------------------ |
   * | identity | Keyword identifying the worker within the current runtime.   |
   * | options  | Optional map with `:environment` and String-vector `:autoloads`. |
   *
   * @return `nil` after the worker runtime has initialized successfully.
   */
  FUNC(CreateWorkerBangFunction, create_worker)

  /*!
   * @brief Schedule worker-side evaluation with parent-evaluated bindings.
   * @since 0.1.0
   *
   * Binding expressions are evaluated sequentially in the parent runtime and
   * copied together into a job-local scope in the named worker. The body is
   * retained as source-accurate AST, lowered against the worker runtime, and
   * evaluated there. Names not introduced by the binding vector therefore
   * resolve only in the worker runtime.
   *
   * Usage:
   * @code
   * (roo.worker/execute-let! :my-app/worker
   *   [data (load-data)]
   *   (process data))
   * => #<roo.worker/execution :my-app/worker 1>
   * @endcode
   *
   * | Arg      | Description                                                   |
   * | -------- | ------------------------------------------------------------- |
   * | identity | Keyword identifying the worker in the current runtime.        |
   * | bindings | Sequential parent-side binding and value-expression pairs.    |
   * | body...  | Forms lowered and evaluated in the worker with those bindings. |
   *
   * @return An execution handle, as accepted by `poll!` and `collect!`.
   */
  SPECIAL_FORM_DECL(ExecuteLetWorkerBangForm, execute_let_worker)

  /*!
   * @brief Schedule a callable invocation in a named worker runtime.
   * @since 0.1.0
   *
   * The callable and arguments are evaluated in the parent runtime, copied
   * together under the runtime-transfer policy, and invoked using the worker's
   * own execution context. The invocation result is copied back before the
   * execution becomes successful. The returned native handle exposes the
   * stable read-only properties `:worker` and `:id` while retaining its
   * runtime-local routing identity.
   *
   * Usage:
   * @code
   * (roo.worker/invoke! :my-app/worker identity {:answer 42})
   * => #<roo.worker/execution :my-app/worker 1>
   * @endcode
   *
   * | Arg      | Description                                                |
   * | -------- | ---------------------------------------------------------- |
   * | identity | Keyword identifying the worker in the current runtime.     |
   * | callable | Callable value to invoke in the worker runtime.            |
   * | args...  | Zero or more argument values transferred to the worker.    |
   *
   * @return An execution handle, as accepted by `poll!` and `collect!`.
   */
  FUNC(InvokeWorkerBangFunction, invoke_worker)

  /*!
   * @brief Return the current status of a worker execution.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (roo.worker/poll! execution)
   * => {:status :queued}
   * (roo.worker/poll! execution {:timeout-ms 80})
   * => {:status :running}
   * @endcode
   *
   * | Arg       | Description                                                  |
   * | --------- | ------------------------------------------------------------ |
   * | execution | Execution handle returned by a worker operation.             |
   * | options   | Optional map with a non-negative integer `:timeout-ms`.      |
   *
   * @return A report map whose `:status` is one of `:queued`, `:running`,
   * `:succeeded`, or `:failed`. Missing options or `:timeout-ms` default to
   * zero and return immediately.
   */
  FUNC(PollWorkerBangFunction, poll_worker)
} // namespace Roo

#endif

#ifndef ROO__LANG__WORKER__WORKER_H
#define ROO__LANG__WORKER__WORKER_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Create and start a named worker owned by the current runtime.
   * @since 0.1.0
   *
   * The worker is registered under a runtime-local keyword identity and owns a
   * dedicated execution thread. Its vanilla Roo runtime is constructed on that
   * thread without application namespaces, namespace loading, or file-system
   * access inherited from the parent. The call returns only after worker
   * initialization succeeds.
   *
   * Creating another worker with the same identity in the current runtime is an
   * error. The same identity may be used independently by another runtime.
   *
   * Usage:
   * @code
   * (roo.worker/create! :my-app/worker) => nil
   * @endcode
   *
   * | Arg      | Description                                                  |
   * | -------- | ------------------------------------------------------------ |
   * | identity | Keyword identifying the worker within the current runtime.   |
   *
   * @return `nil` after the worker runtime has initialized successfully.
   */
  FUNC(CreateWorkerBangFunction, create_worker)
} // namespace Roo

#endif

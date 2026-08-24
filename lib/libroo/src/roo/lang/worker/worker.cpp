#include "roo/lang/worker/worker.h"

#include <roo/context.h>
#include <roo/exec.h>
#include <roo/runtime.h>
#include <roo/runtime/value.h>

namespace Roo
{
  /** CreateWorkerBangFunction - roo.worker/create! */
  FUNC_IMPL(CreateWorkerBangFunction,
            SIG((FN_ARGS((&Type::KEYWORD)),
                 EXEC_DISPATCH(&CreateWorkerBangFunction::exec_create_worker))))

  EXEC_BODY(CreateWorkerBangFunction, exec_create_worker)
  {
    ctx.get_runtime().create_worker(args[0]->str());
    return Constant::NIL;
  }
} // namespace Roo

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

  /** InvokeWorkerBangFunction - roo.worker/invoke! */
  FUNC_IMPL(InvokeWorkerBangFunction,
            SIG((FN_ARGS((&Type::KEYWORD), (&Type::EXEC), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&InvokeWorkerBangFunction::exec_invoke_worker))))

  EXEC_BODY(InvokeWorkerBangFunction, exec_invoke_worker)
  {
    sptr_val_v invocation_arguments(args.begin() + 2, args.end());
    return ctx.get_runtime().invoke_worker(args[0]->str(), args[1], invocation_arguments);
  }

  /** PollWorkerBangFunction - roo.worker/poll! */
  FUNC_IMPL(PollWorkerBangFunction,
            SIG((FN_ARGS((&Type::ANY)),
                 EXEC_DISPATCH(&PollWorkerBangFunction::exec_poll_worker))))

  EXEC_BODY(PollWorkerBangFunction, exec_poll_worker)
  {
    return ctx.get_runtime().poll_worker(args[0]);
  }

  /** CollectWorkerBangFunction - roo.worker/collect! */
  FUNC_IMPL(CollectWorkerBangFunction,
            SIG((FN_ARGS((&Type::ANY)),
                 EXEC_DISPATCH(&CollectWorkerBangFunction::exec_collect_worker))))

  EXEC_BODY(CollectWorkerBangFunction, exec_collect_worker)
  {
    return ctx.get_runtime().collect_worker(args[0]);
  }
} // namespace Roo

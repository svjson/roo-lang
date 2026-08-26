#include "roo/lang/worker/worker_namespace.h"

#include <map>
#include <string>

#include <roo/lang/worker/worker.h>
#include <roo/runtime/value.h>

namespace Roo
{
  Namespace make_worker_namespace()
  {
    std::map<std::string, sptr_val> symbols;
    symbols.emplace("collect!", CollectWorkerBangFunction::make());
    symbols.emplace("create!", CreateWorkerBangFunction::make());
    symbols.emplace("execute-let!", ExecuteLetWorkerBangForm::make());
    symbols.emplace("invoke!", InvokeWorkerBangFunction::make());
    symbols.emplace("poll!", PollWorkerBangFunction::make());

    return Namespace::make_lang("roo.worker", std::move(symbols));
  }
} // namespace Roo

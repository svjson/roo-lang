
#include "roo/lang/time.h"

#include <chrono>

#include <roo/exec.h>
#include <roo/runtime/time.h>

namespace Roo
{
  /** EpochMsFunction - roo/epoch-ms */
  FUNC_IMPL(EpochMsFunction, SIG((NO_ARGS, EXEC_DISPATCH(&EpochMsFunction::exec_epoch_ms))));

  EXEC_BODY(EpochMsFunction, exec_epoch_ms)
  {
    auto now = std::chrono::system_clock::now();
    long long ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return Roo::Value::number(ms);
  }

  /** SleepBangFunction - roo/sleep! */
  FUNC_IMPL(SleepBangFunction,
            SIG((FN_ARGS((&Type::NUMBER)),
                 EXEC_DISPATCH(&SleepBangFunction::exec_sleep))))

  EXEC_BODY(SleepBangFunction, exec_sleep)
  {
    RuntimeTiming::sleep_for(RuntimeTiming::read_milliseconds(args[0], "sleep!"));
    return Constant::NIL;
  }
} // namespace Roo

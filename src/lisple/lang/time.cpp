
#include "lisple/lang/time.h"

#include <chrono>

#include <lisple/exec.h>

namespace Lisple
{
  /** EpochMsFunction - epoch-ms */
  FUNC_IMPL(EpochMsFunction, SIG((NO_ARGS, EXEC_DISPATCH(&EpochMsFunction::exec_epoch_ms))));

  EXEC_BODY(EpochMsFunction, exec_epoch_ms)
  {
    auto now = std::chrono::system_clock::now();
    long ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return Lisple::Value::number(ms);
  }
} // namespace Lisple

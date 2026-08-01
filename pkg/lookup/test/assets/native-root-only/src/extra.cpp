#include "extra.h"

namespace Extra
{
  /** ExtraReadFunction - extra.native/read! */
  FUNC_IMPL(ExtraReadFunction, SIG((NO_ARGS, EXEC_DISPATCH(&ExtraReadFunction::exec_read))))
} // namespace Extra

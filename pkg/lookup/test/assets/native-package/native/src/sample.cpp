#include "sample.h"

namespace Sample
{
  /** SampleReadFunction - sample.native/read! */
  FUNC_IMPL(SampleReadFunction, SIG((NO_ARGS, EXEC_DISPATCH(&SampleReadFunction::exec_read))))
} // namespace Sample

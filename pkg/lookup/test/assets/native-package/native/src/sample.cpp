#include "sample.h"

namespace Sample
{
  /** SampleReadFunction - sample.native/read! */
  FUNC_IMPL(SampleReadFunction, SIG((NO_ARGS, EXEC_DISPATCH(&SampleReadFunction::exec_read))))

  /*!
   * @roo.symbol sample.native/write!
   * @roo.kind function
   * @brief Write fixture data to a path.
   *
   * | Arg  | Description    |
   * | ---- | -------------- |
   * | path | Path to write. |
   */
  ns->store("write!", SampleWriteFunction::make());
} // namespace Sample

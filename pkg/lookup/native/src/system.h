#ifndef LOOKUP_NATIVE_SYSTEM_H
#define LOOKUP_NATIVE_SYSTEM_H

#include <roo/exec.h>

namespace Roo::Lookup
{
  /*!
   * @brief Fail the current lookup command with an error message.
   *
   * This is used by command-line paths that need to emit structured output and
   * then return a non-zero process status.
   *
   * Usage:
   * @code
   * (lookup.system/fail! "lookup audit failed")
   * @endcode
   *
   * | Arg # | Description              |
   * |-------|--------------------------|
   * | 0     | Error message to report. |
   */
  FUNC(FailBangFunction, fail)
} // namespace Roo::Lookup

#endif

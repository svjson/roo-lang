#ifndef LOOKUP_NATIVE_ANALYSIS_H
#define LOOKUP_NATIVE_ANALYSIS_H

#include <roo/exec.h>

namespace Roo::Lookup
{
  /*!
   * @brief Return lookup information for the Roo form at a source position.
   *
   * The result includes the selected thing, its source range, and local binding
   * resolution when the selected thing is a local binding or reference.
   *
   * Usage:
   * @code
   * (lookup.analysis/thing-at! "." "src/app/core.roo" "3" "7" "(ns app.core)")
   * => {:status :ok :thing {...}}
   * @endcode
   *
   * | Arg # | Description                                      |
   * |-------|--------------------------------------------------|
   * | 0     | Package root used by the lookup command.         |
   * | 1     | Source file path.                                |
   * | 2     | One-based source line as a string.               |
   * | 3     | One-based source column as a string.             |
   * | 4     | Complete source text to analyze.                 |
   */
  FUNC(ThingAtFunction, thing_at)
} // namespace Roo::Lookup

#endif

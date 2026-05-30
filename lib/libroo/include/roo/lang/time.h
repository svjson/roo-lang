
#ifndef ROO__LANG__TIME_H
#define ROO__LANG__TIME_H

#include <roo/exec.h>

namespace Roo
{

  /**
   * @brief Get the current time in milliseconds since the Unix epoch.
   *
   * Usage:
   * @code
   * (epoch-ms) => 1776954460835
   * @endcode
   */
  FUNC(EpochMsFunction, epoch_ms);

} // namespace Roo

#endif

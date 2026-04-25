
#ifndef LISPLE__LANG__TIME_H
#define LISPLE__LANG__TIME_H

#include <lisple/exec.h>

namespace Lisple
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

} // namespace Lisple

#endif

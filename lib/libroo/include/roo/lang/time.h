
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

  /**
   * @brief Suspend the current runtime thread for a duration.
   * @since 0.1.0
   *
   * The operating system parks the thread; the function does not busy-spin.
   * Other runtimes, including worker runtimes, continue independently.
   *
   * Usage:
   * @code
   * (sleep! 80) => nil
   * @endcode
   *
   * | Arg            | Description                              |
   * | -------------- | ---------------------------------------- |
   * | milliseconds   | Non-negative integer duration to sleep.  |
   *
   * @return `nil` after the duration has elapsed.
   */
  FUNC(SleepBangFunction, sleep);

} // namespace Roo

#endif

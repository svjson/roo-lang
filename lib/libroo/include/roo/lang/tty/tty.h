#ifndef ROO__LANG__TTY__TTY_H
#define ROO__LANG__TTY__TTY_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Query whether stdout is connected to a real terminal.
   *
   * Usage:
   * @code
   * (roo.tty/tty?) => true
   * @endcode
   */
  FUNC(TtyPFunction, tty_p)

  /*!
   * @brief Query the terminal's current dimensions.
   *
   * The returned map contains :columns and :rows. Returns nil when stdout
   * is not a real terminal, or the dimensions cannot be determined.
   *
   * Usage:
   * @code
   * (roo.tty/dimensions!) => {:columns 80 :rows 24}
   * (roo.tty/dimensions!) => nil
   * @endcode
   */
  FUNC(DimensionsBangFunction, dimensions)
} // namespace Roo

#endif

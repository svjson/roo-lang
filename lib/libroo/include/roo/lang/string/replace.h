#ifndef ROO__LANG__STRING__REPLACE_H
#define ROO__LANG__STRING__REPLACE_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Replace all literal substring occurrences in a string.
   *
   * Usage:
   * @code
   * (roo.string/replace "Normal Club" "Normal" "Enchanted")
   * => "Enchanted Club"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The source string.                                               |
   * | 1     | The literal substring to replace.                                |
   * | 2     | The replacement string.                                          |
   */
  FUNC(StringReplaceFunction, replace)

  /*!
   * @brief Replace the first literal substring occurrence in a string.
   *
   * Usage:
   * @code
   * (roo.string/replace-first "Normal Normal" "Normal" "Enchanted")
   * => "Enchanted Normal"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The source string.                                               |
   * | 1     | The literal substring to replace.                                |
   * | 2     | The replacement string.                                          |
   */
  FUNC(StringReplaceFirstFunction, replace_first)
} // namespace Roo

#endif

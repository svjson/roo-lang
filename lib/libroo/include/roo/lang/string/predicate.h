#ifndef ROO__LANG__STRING__PREDICATE_H
#define ROO__LANG__STRING__PREDICATE_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Tests whether a string ends with a suffix.
   *
   * Usage:
   * @code
   * (roo.string/ends-with? "abcdef" "def") => true
   * (roo.string/ends-with? "abcdef" "bcd") => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The source string.                                               |
   * | 1     | The suffix to match.                                             |
   */
  FUNC(StringEndsWithPFunction, ends_with_p)

  /*!
   * @brief Tests whether a string starts with a prefix.
   *
   * Usage:
   * @code
   * (roo.string/starts-with? "abcdef" "abc") => true
   * (roo.string/starts-with? "abcdef" "bcd") => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The source string.                                               |
   * | 1     | The prefix to match.                                             |
   */
  FUNC(StringStartsWithPFunction, starts_with_p)
} // namespace Roo

#endif

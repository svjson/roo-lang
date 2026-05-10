
#ifndef LISPLE__LANG__STRING_H
#define LISPLE__LANG__STRING_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Converts any object to their string representation and concatenates
   * multiple forms or strings to a single string
   *
   * Usage:
   * @code
   * (str 1) => "1"
   * (str "There is no " (rand-nth ["spoon" "hope"])) => "There is no spoon"
   * (str :regular-key) => ":regular-key"
   * (str "The number " x " is the value of x")
   * => "The number 10 is the value of x"
   * @endcode
   */
  FUNC(StrFunction, str)

  /*!
   * @brief Joins multiple strings with a separator.
   *
   * Usage:
   * @code
   * (join ", " "a" "b" "c") => "a, b, c"
   * @endcode
   */
  FUNC(JoinFunction, join)

  /*!
   * @brief Extracts a substring from a string.
   *
   * Usage:
   * @code
   * (substr "abcdef" 2) => "cdef"
   * (substr "abcdef" 2 3) => "cde"
   * @endcode
   */
  FUNC(SubstrFunction, substr)

  /*!
   * @brief Removes leading and trailing whitespace from a string.
   *
   * Usage:
   * @code
   * (trim "  hello  ") => "hello"
   * @endcode
   */
  FUNC(TrimFunction, trim)

  /*!
   * @brief Tests whether a string is empty or contains only whitespace.
   *
   * Usage:
   * @code
   * (blank? "") => true
   * (blank? " \n\t ") => true
   * (blank? nil) => true
   * @endcode
   */
  FUNC(BlankPFunction, blank_p)

  /*!
   * @brief Converts string or string representation to all upper-case.
   *
   * Usage:
   * @code
   * (upper-case "mIxEd-CaSe!") => "MIXED-CASE!"
   * (upper-case :regular-key) => ":REGULAR-KEY"
   * @endcode
   */
  FUNC(UpperCaseFunction, upper_case)

  /*!
   * @brief Converts string or string representation to all lower-case.
   *
   * Usage:
   * @code
   * (lower-case "mIxEd-CaSe!") => "mixed-case!"
   * (lower-case :UPCASE-KEY) => ":upcase-key"
   * @endcode
   */
  FUNC(LowerCaseFunction, lower_case)

} // namespace Lisple

#endif /* LISPLE__LANG__STRING_H */

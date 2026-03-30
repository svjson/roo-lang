
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

} // namespace Lisple

#endif /* LISPLE__LANG__STRING_H */

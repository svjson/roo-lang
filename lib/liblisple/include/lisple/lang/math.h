
#ifndef LISPLE__LANG__MATH_H
#define LISPLE__LANG__MATH_H

#include <lisple/exec.h>

namespace Lisple
{

  /*!
   * @brief Returns the absolute value of a Number
   *
   * Usage:
   * @code
   * (abs -5)
   * => 5
   *
   * (abs 5)
   * => 5
   *
   * (abs -5.5)
   * => 5.5
   *
   * (abs 5.5)
   * => 5.5
   *
   * (abs nil)
   * => nil
   * @endcode
   */
  FUNC(AbsFunction, abs)

  /*!
   * @brief Rounds a number up to the nearest integer
   *
   * Usage:
   * @code
   * (ceil 10.2)
   * => 11
   *
   * (ceil 10.0)
   * => 10
   *
   * (ceil nil)
   * => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The number to round up                                           |
   */
  FUNC(CeilFunction, ceil)

  FUNC(CosFunction, cos);
  FUNC(SinFunction, sin);
  FUNC(SqrtFunction, sqrt);

} // namespace Lisple

#endif /* LISPLE__LANG__MATH_H */

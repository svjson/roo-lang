
#ifndef ROO__LANG__MATH_H
#define ROO__LANG__MATH_H

#include <roo/exec.h>

namespace Roo
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

  /*!
   * @brief Return the cosine of a number.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Angle in radians.                                                |
   */
  FUNC(CosFunction, cos);

  /*!
   * @brief Round a number down to the nearest integer.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The number to round down.                                        |
   */
  FUNC(FloorFunction, floor)

  /*!
   * @brief Raise a number to the power of another number.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Base number.                                                     |
   * | 1     | Exponent number.                                                 |
   */
  FUNC(PowFunction, pow);

  /*!
   * @brief Round a number to the nearest integer.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The number to round.                                             |
   */
  FUNC(RoundFunction, round)

  /*!
   * @brief Return the sine of a number.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Angle in radians.                                                |
   */
  FUNC(SinFunction, sin);

  /*!
   * @brief Return the square root of a number.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The number whose square root is returned.                        |
   */
  FUNC(SqrtFunction, sqrt);

} // namespace Roo

#endif /* ROO__LANG__MATH_H */

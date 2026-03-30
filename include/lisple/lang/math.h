
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
   * @endcode
   */
  FUNC(AbsFunction, abs)
  FUNC(CosFunction, cos);
  FUNC(SinFunction, sin);
  FUNC(SqrtFunction, sqrt);

} // namespace Lisple

#endif /* LISPLE__LANG__MATH_H */


#ifndef LISPLE__LANG__ARITHMETIC_H
#define LISPLE__LANG__ARITHMETIC_H

#include "../exec.h"
#include <functional>

namespace Lisple
{
  /**
   * @brief Adds any number of numeric arguments together, and returns the sum.
   *
   * Usage:
   * @code
   * (+ 1 2 3)
   * => 6
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0...  | The numbers to add together. Must be at least one argument.      |
   */
  FUNC(PlusFunction, plus)

  /**
   * @brief Subtracts any number of numeric arguments from the first argument, and
   * returns the difference.
   *
   * Usage:
   * @code
   * (- 8 3 2)
   * => 3
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0...  | The numbers to subtract. Must be at least one argument.          |
   */
  FUNC_DECL(MinusFunction, do_subtraction)

  /**
   * @brief Divides the first numeric argument by the second, and then divides the
   * result by each subsequent argument, returning the quotient.
   *
   * Usage:
   * @code
   * (* 80 2 10)
   * => 4
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0...  | The numbers to divide. Must be at least one argument.            |
   */
  FUNC_DECL(DivideFunction, do_division)

  /**
   * @brief Multiplies any number of numeric arguments together, and
   * returns the product.
   *
   * Usage:
   * @code
   * (* 4 2 10)
   * => 80
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0...  | The numbers to multiply. Must be at least one argument.          |
   */
  FUNC_DECL(MultiplyFunction, do_multiplication)
} // namespace Lisple

#endif /* LISPLE__LANG__ARITHMETIC_H */

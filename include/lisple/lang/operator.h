
#ifndef LISPLE__LANG__ARITHMETIC_H
#define LISPLE__LANG__ARITHMETIC_H

#include <lisple/exec.h>

namespace Lisple
{
  /**
   * @brief Test equality of two values
   *
   * Usage:
   * @code
   * (= 10 5)
   * => false
   *
   * (= 10 10)
   * => true
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Left hand side value.                                            |
   * | 1     | Right hand side value.                                           |
   */
  FUNC(EqualsPredicateFunction, equals)

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
  FUNC(MinusFunction, subtract)

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
  FUNC(DivideFunction, divide)

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
  FUNC(MultiplyFunction, multiply)

  /*!
   * @brief Performs numerical or lexical comparison between two values.
   *
   * Usage:
   * @code
   * (< 5 10)
   * => true
   *
   * (< 10 5)
   * => false
   *
   * (< 10 10)
   * => false
   * @endcode
   */
  FUNC(LessThanFunction, lt_num, lt_str)

  /*!
   * @brief Performs numerical or lexical comparison between two
   * values.
   *
   * Usage:
   * (<= 5 10)
   * => true
   *
   * (<= 10 5)
   * => false
   *
   * (<= 10 10)
   * => true
   * @endcode
   */
  FUNC(LessThanOrEqualsFunction, lte_num, lte_str)

  /*!
   * @brief Performs numerical or lexical comparison between two values.
   *
   * Usage:
   * @code
   * (> 5 10)
   * => false
   *
   * (> 10 5)
   * => true
   *
   * (> 10 10)
   * => false
   *
   * (> "hat" "cat")
   * => true
   *
   * @endcode
   */
  FUNC(GreaterThanFunction, gt_num, gt_str)

  /*!
   * @brief Performs numerical or lexical comparison between two values.
   *
   * Usage:
   * @code
   * (>= 5 10)
   * => false
   *
   * (>= 10 5)
   * => true
   *
   * (>= 10 10)
   * => true
   * @endcode
   */
  FUNC(GreaterThanOrEqualsFunction, gte_num, gte_str)

} // namespace Lisple

#endif /* LISPLE__LANG__ARITHMETIC_H */

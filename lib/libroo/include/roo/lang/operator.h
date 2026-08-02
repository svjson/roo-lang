
#ifndef ROO__LANG__ARITHMETIC_H
#define ROO__LANG__ARITHMETIC_H

#include <roo/exec.h>

namespace Roo
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
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | left  | Left hand side value.                                              |
   * | right | Right hand side value.                                             |
   */
  FUNC(EqualsPFunction, equals)

  /**
   * @brief Adds any number of numeric arguments together, and returns the sum.
   *
   * Usage:
   * @code
   * (+ 1 2 3)
   * => 6
   * @endcode
   *
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | The numbers to add together. Must be at least one argument.        |
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
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | The numbers to subtract. Must be at least one argument.            |
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
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | The numbers to divide. Must be at least one argument.              |
   */
  FUNC(DivideFunction, divide)

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
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | The numbers to divide. Must be at least one argument.              |
   */
  FUNC(ModulusFunction, modulus)

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
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | The numbers to multiply. Must be at least one argument.            |
   */
  FUNC(MultiplyFunction, multiply)

  /**
   * @brief Shifts an integer value left by one bit, or by the given number of bits.
   *
   * `<<` is the signed/arithmetic left shift operator. It accepts integer numbers
   * only; floating point values are rejected. The optional shift amount defaults to
   * `1` and must be between `0` and `63`, inclusive.
   *
   * Usage:
   * @code
   * (<< 8)
   * => 16
   *
   * (<< 8 2)
   * => 32
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Integer value to shift.                                            |
   * | bits  | Optional integer number of bits to shift by. Defaults to 1.        |
   */
  FUNC(ShiftLeftFunction, shift_left)

  /**
   * @brief Shifts an integer value right by one bit, or by the given number of bits.
   *
   * `>>` is the signed/arithmetic right shift operator. Negative values keep their
   * sign through the shift. It accepts integer numbers only; floating point values
   * are rejected. The optional shift amount defaults to `1` and must be between `0`
   * and `63`, inclusive.
   *
   * Usage:
   * @code
   * (>> 8)
   * => 4
   *
   * (>> 8 3)
   * => 1
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Integer value to shift.                                            |
   * | bits  | Optional integer number of bits to shift by. Defaults to 1.        |
   */
  FUNC(ShiftRightFunction, shift_right)

  /**
   * @brief Shifts an integer bit pattern left by one bit, or by the given number of bits.
   *
   * `<<<` is the logical/bit-pattern left shift operator. Left shifts do not perform
   * sign extension, so this currently has the same bit-level result as `<<`; it is
   * provided as the logical counterpart to `>>>`. It accepts integer numbers only;
   * floating point values are rejected. The optional shift amount defaults to `1`
   * and must be between `0` and `63`, inclusive.
   *
   * Usage:
   * @code
   * (<<< 8)
   * => 16
   *
   * (<<< 8 2)
   * => 32
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Integer bit pattern to shift.                                      |
   * | bits  | Optional integer number of bits to shift by. Defaults to 1.        |
   */
  FUNC(LogicalShiftLeftFunction, logical_shift_left)

  /**
   * @brief Shifts an integer bit pattern right by one bit, or by the given number of bits.
   *
   * `>>>` is the logical/unsigned right shift operator. It treats the input as a
   * 64-bit bit pattern and shifts zero bits in from the left. It accepts integer
   * numbers only; floating point values are rejected. The optional shift amount
   * defaults to `1` and must be between `0` and `63`, inclusive.
   *
   * Usage:
   * @code
   * (>>> 8)
   * => 4
   *
   * (>>> -1 1)
   * => 9223372036854775807
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Integer bit pattern to shift.                                      |
   * | bits  | Optional integer number of bits to shift by. Defaults to 1.        |
   */
  FUNC(LogicalShiftRightFunction, logical_shift_right)

  /**
   * @brief Computes the bitwise AND of one or more integer values.
   *
   * `bit-and` accepts integer numbers only; floating point values are rejected. At
   * least one argument is required. All operands are treated as 64-bit integer bit
   * patterns, and the result is returned as an integer number.
   *
   * Usage:
   * @code
   * (bit-and 6 3)
   * => 2
   *
   * (bit-and 15 7 3)
   * => 3
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | values... | Integer values to combine. Must be at least one argument.          |
   */
  FUNC(BitAndFunction, bit_and)

  /**
   * @brief Computes the bitwise OR of one or more integer values.
   *
   * `bit-or` accepts integer numbers only; floating point values are rejected. At
   * least one argument is required. All operands are treated as 64-bit integer bit
   * patterns, and the result is returned as an integer number.
   *
   * Usage:
   * @code
   * (bit-or 4 2)
   * => 6
   *
   * (bit-or 8 4 1)
   * => 13
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | values... | Integer values to combine. Must be at least one argument.          |
   */
  FUNC(BitOrFunction, bit_or)

  /**
   * @brief Computes the bitwise XOR of one or more integer values.
   *
   * `bit-xor` accepts integer numbers only; floating point values are rejected. At
   * least one argument is required. All operands are treated as 64-bit integer bit
   * patterns, and the result is returned as an integer number.
   *
   * Usage:
   * @code
   * (bit-xor 6 3)
   * => 5
   *
   * (bit-xor 7 3 1)
   * => 5
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | values... | Integer values to combine. Must be at least one argument.          |
   */
  FUNC(BitXorFunction, bit_xor)

  /**
   * @brief Computes the bitwise complement of an integer value.
   *
   * `bit-not` accepts integer numbers only; floating point values are rejected. The
   * operand is treated as a 64-bit integer bit pattern, and the result is returned
   * as an integer number.
   *
   * Usage:
   * @code
   * (bit-not 0)
   * => -1
   *
   * (bit-not 10)
   * => -11
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Integer value to complement.                                       |
   */
  FUNC(BitNotFunction, bit_not)

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

} // namespace Roo

#endif /* ROO__LANG__ARITHMETIC_H */

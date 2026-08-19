#ifndef ROO__LANG__NUM_H
#define ROO__LANG__NUM_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Clamp a number between lower and upper bounds.
   * @since 0.1.0
   * @see roo/clamp-between
   * @see roo/min
   * @see roo/max
   *
   * The `min` and `max` arguments always keep their positional meaning. They
   * may independently be `nil`: a `nil` minimum applies only the upper bound,
   * a `nil` maximum applies only the lower bound, and two `nil` bounds return
   * the value unchanged. Bounds are not automatically reversed.
   *
   * Usage:
   * @code
   * (clamp 5 0 10)
   * => 5
   *
   * (clamp -3 0 10)
   * => 0
   *
   * (clamp 15 0 10)
   * => 10
   *
   * (clamp 12 nil nil)
   * => 12
   *
   * (clamp 12 10 nil)
   * => 12
   * @endcode
   *
   * | Arg   | Description                                      |
   * | ----- | ------------------------------------------------ |
   * | value | The number to clamp.                             |
   * | min   | The inclusive lower bound, or `nil`.             |
   * | max   | The inclusive upper bound, or `nil`.             |
   *
   * @return The clamped value, or `nil` when `value` is `nil`.
   */
  FUNC(ClampFunction, clamp)

  /*!
   * @brief Clamp a number between two inclusive bounds, in either order.
   * @since 0.1.0
   * @see roo/min
   * @see roo/max
   *
   * The bounds can be passed in either order. A `nil` value produces `nil`.
   * If either bound is `nil`, the value is returned unchanged because there is
   * no complete range to apply.
   *
   * Usage:
   * @code
   * (clamp-between 5 0 10)
   * => 5
   *
   * (clamp-between -3 0 10)
   * => 0
   *
   * (clamp-between 15 0 10)
   * => 10
   *
   * (clamp-between 15 10 0)
   * => 10
   *
   * (clamp-between nil 0 10)
   * => nil
   *
   * (clamp-between 12 nil nil)
   * => 12
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | The number to clamp.                                               |
   * | low   | One inclusive range bound.                                         |
   * | high  | The other inclusive range bound.                                   |
   *
   * @return The clamped value, or `nil` when an argument is `nil`.
   */
  FUNC(ClampBetweenFunction, clamp_between)

  /*!
   * @brief Return the integer code point of a character.
   * @since 0.1.0
   * @see roo/int
   *
   * `codepoint` reads the character value directly rather than parsing its
   * textual representation as a number.
   *
   * Usage:
   * @code
   * (codepoint 'g')
   * => 103
   *
   * (codepoint '8')
   * => 56
   * @endcode
   *
   * | Arg       | Description                               |
   * | --------- | ----------------------------------------- |
   * | character | The character whose code point to return. |
   *
   * @return The character's code point as an integer.
   */
  FUNC(CodepointFunction, codepoint)

  /*!
   * @brief Decrement a number by one.
   * @since 0.1.0
   * @see roo/inc
   *
   * Returns `nil` when the input is `nil`.
   *
   * Usage:
   * @code
   * (dec 2)
   * => 1
   *
   * (dec nil)
   * => nil
   * @endcode
   *
   * | Arg    | Description                 |
   * | ------ | --------------------------- |
   * | number | The number to decrement.    |
   *
   * @return The decremented number, or `nil`.
   */
  FUNC(DecFunction, dec)

  /*!
   * @brief Test whether values represent a single decimal digit.
   * @since 0.1.0
   *
   * A character is a digit when it is between `0` and `9`. Strings, symbols,
   * and keywords must contain exactly one such character. Numeric values must
   * be integer numbers between 0 and 9; floating point numbers are not digits.
   * All arguments must satisfy the predicate.
   *
   * Usage:
   * @code
   * (digit? '8' "4" :1 '7 3)
   * => true
   *
   * (digit? "12" 8.0)
   * => false
   * @endcode
   *
   * | Arg        | Description                         |
   * | ---------- | ----------------------------------- |
   * | values...  | Values to test as decimal digits.  |
   *
   * @return `true` when every argument is a digit; otherwise `false`.
   */
  FUNC(DigitPFunction, digitp)

  /*!
   * @brief Check whether a number is even.
   * @since 0.1.0
   * @see roo/odd?
   *
   * Usage:
   * @code
   * (even? 2)
   * => true
   * @endcode
   *
   * | Arg    | Description                  |
   * | ------ | ---------------------------- |
   * | number | The number to test.          |
   *
   * @return `true` when the number is even; otherwise `false`.
   */
  FUNC(EvenPFunction, even)

  /*!
   * @brief Convert a value to a floating point number.
   * @since 0.1.0
   * @see roo/int
   * @see roo/number
   *
   * Strings, keywords, symbols, and characters are parsed as numeric text.
   * Values that cannot be converted return `nil`.
   *
   * Usage:
   * @code
   * (float "48.12")
   * => 48.12
   *
   * (float :-1)
   * => -1.0
   * @endcode
   *
   * | Arg   | Description                |
   * | ----- | -------------------------- |
   * | value | The value to convert.      |
   *
   * @return A floating point value, or `nil` when conversion is not possible.
   */
  FUNC(FloatFunction, to_float)

  /*!
   * @brief Test whether one or more values are floating point numbers.
   * @since 0.1.0
   * @see roo/int?
   * @see roo/number?
   *
   * The predicate returns `true` only when every argument is represented as a
   * floating point number. Integer numbers and values that could be converted
   * to floats are not floating point numbers.
   *
   * Usage:
   * @code
   * (float? 1.0 2.5)
   * => true
   *
   * (float? 1 2.0)
   * => false
   * @endcode
   *
   * | Arg        | Description                  |
   * | ---------- | ---------------------------- |
   * | numbers... | Values to test.              |
   *
   * @return `true` when every argument is a floating point number; otherwise `false`.
   */
  FUNC(FloatPFunction, floatp)

  /*!
   * @brief Increment a number by one.
   * @since 0.1.0
   * @see roo/dec
   *
   * Returns `nil` when the input is `nil`.
   *
   * Usage:
   * @code
   * (inc 2)
   * => 3
   *
   * (inc nil)
   * => nil
   * @endcode
   *
   * | Arg    | Description                 |
   * | ------ | --------------------------- |
   * | number | The number to increment.    |
   *
   * @return The incremented number, or `nil`.
   */
  FUNC(IncFunction, inc)

  /*!
   * @brief Convert a value to an integer.
   * @since 0.1.0
   * @see roo/float
   * @see roo/number
   *
   * Numeric values are converted by truncating toward zero. Strings,
   * keywords, symbols, and characters are parsed as numeric text. Values that
   * cannot be converted return `nil`.
   *
   * Usage:
   * @code
   * (int 4.5)
   * => 4
   *
   * (int "48.12")
   * => 48
   *
   * (int :-1)
   * => -1
   * @endcode
   *
   * | Arg   | Description                |
   * | ----- | -------------------------- |
   * | value | The value to convert.      |
   *
   * @return An integer value, or `nil` when conversion is not possible.
   */
  FUNC(IntFunction, to_int)

  /*!
   * @brief Test whether one or more values are integer numbers.
   * @since 0.1.0
   * @see roo/float?
   * @see roo/number?
   *
   * The predicate returns `true` only when every argument is represented as an
   * integer number. Floating point numbers and values that could be converted
   * to integers are not integer numbers.
   *
   * Usage:
   * @code
   * (int? 1 2)
   * => true
   *
   * (int? 1 2.0)
   * => false
   * @endcode
   *
   * | Arg        | Description                  |
   * | ---------- | ---------------------------- |
   * | numbers... | Values to test.              |
   *
   * @return `true` when every argument is an integer number; otherwise `false`.
   */
  FUNC(IntPFunction, intp)

  /*!
   * @brief Return the largest numeric argument.
   * @since 0.1.0
   * @see roo/min
   *
   * `nil` arguments are ignored. The function returns `nil` when no usable
   * numeric argument remains.
   *
   * Usage:
   * @code
   * (max 4 2 9)
   * => 9
   * @endcode
   *
   * | Arg        | Description                    |
   * | ---------- | ------------------------------ |
   * | numbers... | Numbers to compare.            |
   *
   * @return The largest number, or `nil` when no number is supplied.
   */
  FUNC(MaxFunction, max)

  /*!
   * @brief Return the smallest numeric argument.
   * @since 0.1.0
   * @see roo/max
   *
   * `nil` arguments are ignored. The function returns `nil` when no usable
   * numeric argument remains.
   *
   * Usage:
   * @code
   * (min 4 2 9)
   * => 2
   * @endcode
   *
   * | Arg        | Description                    |
   * | ---------- | ------------------------------ |
   * | numbers... | Numbers to compare.            |
   *
   * @return The smallest number, or `nil` when no number is supplied.
   */
  FUNC(MinFunction, min)

  /*!
   * @brief Convert a value to its numeric representation.
   * @since 0.1.0
   * @see roo/int
   * @see roo/float
   *
   * Existing numbers are returned unchanged. Numeric text is returned as an
   * integer when it is integer-shaped and as a float otherwise. Characters are
   * parsed as their one-character textual contents. Unsupported or malformed
   * values return `nil`.
   *
   * Usage:
   * @code
   * (number "489")
   * => 489
   *
   * (number "48.12")
   * => 48.12
   * @endcode
   *
   * | Arg   | Description                |
   * | ----- | -------------------------- |
   * | value | The value to convert.      |
   *
   * @return The existing or parsed number, or `nil` when conversion fails.
   */
  FUNC(NumberFunction, number)

  /*!
   * @brief Test whether one or more values are numbers.
   * @since 0.1.0
   * @see roo/int
   * @see roo/number
   *
   * Usage:
   * @code
   * (number? 2 3.5 -4)
   * => true
   *
   * (number? 2 "a string")
   * => false
   * @endcode
   *
   * | Arg       | Description                                      |
   * | --------- | ------------------------------------------------ |
   * | values... | The values to test. Must be at least one value.  |
   *
   * @return `true` when every value is numeric; otherwise `false`.
   */
  FUNC(NumberPFunction, num)

  /*!
   * @brief Check whether a number is odd.
   * @since 0.1.0
   * @see roo/even?
   *
   * Usage:
   * @code
   * (odd? 2)
   * => false
   * @endcode
   *
   * | Arg    | Description                  |
   * | ------ | ---------------------------- |
   * | number | The number to test.          |
   *
   * @return `true` when the number is odd; otherwise `false`.
   */
  FUNC(OddPFunction, odd)

  /*!
   * @brief Check one or more numbers for equality with zero.
   * @since 0.1.0
   * @see roo/number?
   *
   * Usage:
   * @code
   * (zero? 0)
   * => true
   *
   * (zero? 0.0 0 0.000)
   * => true
   *
   * (zero? 0 1)
   * => false
   * @endcode
   *
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | The numbers to test for equality with zero. Must be at least one   |
   * |            | argument.                                                          |
   *
   * @return `true` when every number is zero; otherwise `false`.
   */
  FUNC(ZeroPFunction, zero)

} // namespace Roo

#endif

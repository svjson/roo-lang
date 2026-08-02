#ifndef ROO__LANG__NUM_H
#define ROO__LANG__NUM_H

#include <roo/exec.h>

namespace Roo
{
  /**
   * @brief Decrement a number by one.
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
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | number | The number to decrement                                            |
   */
  FUNC(DecFunction, dec)

  /**
   * @brief Check if a number is even
   *
   * Usage:
   * @code
   * (even? 2)
   * => true
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | number | The number to test for evenness                                    |
   */
  FUNC(EvenPFunction, even)

  /**
   * @brief Increment a number by one.
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
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | number | The number to increment                                            |
   */
  FUNC(IncFunction, inc)

  /*!
   * @brief Convert a numeric value to an integer.
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Numeric value to convert.                                          |
   */
  FUNC(IntFunction, to_int)

  /*!
   * @brief Convert a numeric value to a floating point number.
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Numeric value to convert.                                          |
   */
  FUNC(FloatFunction, to_float)

  /*!
   * @brief Return the smallest numeric argument.
   *
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | Numbers to compare.                                                |
   */
  FUNC(MinFunction, min)

  /*!
   * @brief Return the largest numeric argument.
   *
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | numbers... | Numbers to compare.                                                |
   */
  FUNC(MaxFunction, max)

  /**
   * @brief Query the type of a value, and return true if it is a number and
   * otherwise false
   *
   * Usage:
   * @code
   * (number? 2)
   * => true
   *
   * (number? "a string")
   * => false
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | The value to test for being a number                               |
   */
  FUNC(NumberPFunction, num)

  /**
   * @brief Check if a number is odd
   *
   * Usage:
   * @code
   * (odd? 2)
   * => false
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | number | The number to test for oddness                                     |
   */
  FUNC(OddPFunction, odd)

  /*!
   * @brief Check one or more numbers for equality with zero
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
   */
  FUNC(ZeroPFunction, zero)
} // namespace Roo

#endif /* ROO__LANG__NUM_H */

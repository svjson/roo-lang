#ifndef LISPLE__LANG__NUM_H
#define LISPLE__LANG__NUM_H

#include <lisple/exec.h>

namespace Lisple
{
  /**
   * @brief Check if a number is even
   *
   * Usage:
   * @code
   * (even? 2)
   * => true
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The number to test for evenness                                  |
   */
  FUNC(EvenPFunction, even)

  /**
   * @brief Check if a number is odd
   *
   * Usage:
   * @code
   * (odd? 2)
   * => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The number to test for oddness                                   |
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
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0...  | The numbers to test for equality with zero. Must be at least one |
   * |       | argument.                                                        |
   */
  FUNC(ZeroPFunction, zero)
} // namespace Lisple

#endif /* LISPLE__LANG__NUM_H */

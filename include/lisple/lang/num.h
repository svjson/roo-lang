#ifndef LISPLE__LANG__NUM_H
#define LISPLE__LANG__NUM_H

#include <lisple/exec.h>

namespace Lisple
{
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

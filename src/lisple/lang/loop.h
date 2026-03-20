#ifndef LISPLE__LANG__LOOP_H
#define LISPLE__LANG__LOOP_H

#include "../exec.h"
#include "loop.h"

namespace Lisple
{
  /**
   * @brief Execute one or more forms n number of times, optionally binding
   * the iteration number to a symbol.
   *
   * Returns a vector containing the results of each iteration.
   *
   * Usage:
   * @code
   * (dotimes [10] (perform-task))
   * (dotimes [n 10] (perform-task n))
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | A binding form or number of iterations                           |
   * | 1...  | Body consisting of one or more forms to be executed.             |
   */
  SPECIAL_FORM_DECL(DoTimesForm, dotimes);

} // namespace Lisple

#endif

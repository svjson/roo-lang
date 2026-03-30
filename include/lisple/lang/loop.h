#ifndef LISPLE__LANG__LOOP_H
#define LISPLE__LANG__LOOP_H

#include <lisple/exec.h>

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

  /*!
   * @brief Executes a form for every element in a sequence, binding the element
   * to the local scope. Returns an array constructed from the return value of
   * each iteration
   *
   * Usage:
   * @code
   * (for [num [1 2 3 4]] (* 2 num)) => [2 4 6 8]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Binding form, [<var-name> <seq>]                                 |
   * | 0     | Form body to execute                                             |
   */
  SPECIAL_FORM_DECL(ForForm, for)

} // namespace Lisple

#endif

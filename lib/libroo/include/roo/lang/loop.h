#ifndef ROO__LANG__LOOP_H
#define ROO__LANG__LOOP_H

#include <roo/exec.h>

namespace Roo
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
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | binding | A binding form or number of iterations                             |
   * | body... | Body consisting of one or more forms to be executed.               |
   */
  SPECIAL_FORM_DECL(DoTimesForm, dotimes);

  /*!
   * @brief Executes a form for every element in a sequence, binding the element
   * to the local scope. Returns an vector constructed from the return value of
   * each iteration
   *
   * Usage:
   * @code
   * (for [num [1 2 3 4]] (* 2 num)) => [2 4 6 8]
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | binding | Binding form, [<var-name> <seq>]                                   |
   * | body    | Form body to execute                                               |
   */
  SPECIAL_FORM_DECL(ForForm, for)

  /*!
   * @brief Executes body forms for every element in a sequence, binding the
   * element to the local scope. Returns nil and does not retain per-iteration
   * return values.
   *
   * Usage:
   * @code
   * (doseq [num [1 2 3 4]] (prn num)) => nil
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | binding | Binding form, [<var-name> <seq>]                                   |
   * | body... | Form body to execute                                               |
   */
  SPECIAL_FORM_DECL(DoseqForm, doseq)

  /*!
   * @brief Executes a form for every element in a sequence, just like
   * (for ...), but takes a leading binding in the binding form containing
   * the zero-based iteration index
   *
   * Usage:
   * @code
   * (for-indexed [index num [1 2 3 4]]
   *   (* index num) => [0 2 6 12])
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | binding | Binding form, [<index-var-name> <var-name> <seq>]                  |
   * | body    | Form body to execute                                               |
   */
  SPECIAL_FORM_DECL(ForIndexedForm, for_indexed)

  /*!
   * @brief Repeatedly evaluate body forms while a condition remains truthy.
   *
   * Usage:
   * @code
   * (while condition
   *   body...)
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | condition | Conditional form tested before each iteration                      |
   * | body...   | Body forms evaluated while the condition is truthy                 |
   */
  SPECIAL_FORM_DECL(WhileForm, while)

} // namespace Roo

#endif

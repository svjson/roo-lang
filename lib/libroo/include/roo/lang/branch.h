
#ifndef ROO__LANG__BRANCH_H
#define ROO__LANG__BRANCH_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief branching construct based on testing a single value against an
   * arbitrary number of constant values.
   *
   * The condition and expression pairs
   * will be evaluated sequentially until a match is found.
   * Optionally, :default can be provided as a last condition that will always
   * evaluate to true if no other match has been made.
   *
   * Usage:
   * @code
   * (case x
   *   10 "x is equal to 10"
   *   20 "x is equal to 20"
   *   :default "x is not 10 or 20")
   * @endcode
   *
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | value      | The value to match                                                 |
   * | match      | A value or expression to match against value                       |
   * | result     | An expression evaluated if match equals value                      |
   * | clauses... | Repetitions of match and result                                    |
   */
  SPECIAL_FORM_DECL(CaseForm, case)

  /*!
   * @brief Defines a sequence of condition and expression pairs that will be
   * sequentially evaluated until a condition evaluates to a truthy, upon
   * which the corresponding expression will be evaluated and returned.
   *
   * @return nil if no condition evaluates to a truthy value
   *
   * Usage:
   * @code
   * (cond
   *   (= x 10) "x is equal to 10"
   *   (> x 10) "x is greater than 10"
   *   :else "x is less than 10")
   * @endcode
   *
   * In the example above, :else is a truthy value and can therefore be used
   * as a last catch-all condition.
   *
   * | Arg        | Description                                                        |
   * | ---------- | ------------------------------------------------------------------ |
   * | condition  | A conditional form                                                 |
   * | result     | An expression that will, conditionally, be evaluated               |
   * | clauses... | Repetitions of condition and result                                |
   */
  SPECIAL_FORM_DECL(CondForm, cond)

  /*!
   * @brief Conditionally evaluate one of two forms.
   *
   * Usage:
   * @code
   * (if condition
   *   :then
   *   :else)
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | condition | Conditional form tested for truthiness                             |
   * | then      | Form evaluated when the condition is truthy                        |
   * | else      | Optional form evaluated when the condition is falsy                |
   */
  SPECIAL_FORM_DECL(IfForm, if)

  /*!
   * @brief Optionally execute forms if conditional expression evaluates
   * to a falsy value.
   *
   * Complement of `when`. If multiple forms are provided after the
   * conditional, they are evaluated as if implicitly wrapped in a
   * (do form1 form2 ...) form.
   *
   * Usage:
   * @code
   * (unless condition
   *   (prn "It's false!"))
   * @endcode
   *
   * | Arg       | Description                                                          |
   * | --------- | -------------------------------------------------------------------- |
   * | condition | A conditional form - anything that can be considered truthy or falsy |
   * | body...   | Any number of forms to be evaluated if the conditional evaluates to  |
   * |           | a falsy value                                                        |
   */
  SPECIAL_FORM_DECL(UnlessForm, unless)

  /*!
   * @brief Optionally execute forms if conditional expression evaluates
   * to a truthy value.
   *
   * If multiple forms are provided after the conditional, they are evaluated as
   * if implicitly wrapped in a (do form1 form2 ...) form.
   *
   * Usage:
   * @code
   * (when condition
   *   (prn "It's true!"))
   *
   * (when condition
   *   (prn "It's true!")
   *   {:result "successful"})
   *
   * (when my-var
   *   (my-fun))
   * @endcode
   *
   * | Arg       | Description                                                          |
   * | --------- | -------------------------------------------------------------------- |
   * | condition | A conditional form - anything that can be considered truthy or falsy |
   * | body...   | Any number of forms to be evaluated if the conditional evaluates to  |
   * |           | a truthy value                                                       |
   */
  SPECIAL_FORM_DECL(WhenForm, when)

} // namespace Roo

#endif /* ROO__LANG__BRANCH_H */

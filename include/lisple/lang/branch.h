
#ifndef LISPLE__LANG__BRANCH_H
#define LISPLE__LANG__BRANCH_H

#include <lisple/exec.h>

namespace Lisple
{
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
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | A conditional form                                               |
   * | 1     | An expression that will, conditionally, be evaluated             |
   * | 2...  | Repetitions of Params 0-1                                        |
   */
  SPECIAL_FORM_DECL(CondForm, cond)

  SPECIAL_FORM_DECL(IfForm, if)

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
   * | Arg # | Description                                                |
   * |-------|------------------------------------------------------------|
   * | 0     | A conditional form - anything that can be considered truthy or falsy |
   * | 1...  | Any number of forms to be evaluated if the conditional evaluates to a truthy
   * value |
   */
  SPECIAL_FORM_DECL(WhenForm, when)

} // namespace Lisple

#endif /* LISPLE__LANG__BRANCH_H */

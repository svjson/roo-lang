#ifndef ROO__LANG__BIND_FORM_H
#define ROO__LANG__BIND_FORM_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Temporarily bind values to variables that will exist only within
   * the scope of the body of the let macro
   *
   * Usage:
   * @code
   * (let [var-name (some-function)] body...)
   * (let [name "Cheech" age 54] body...)
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | bindings | Vector containing a variable name and variable value pairs         |
   * | body...  | Body forms to execute with the temporary bindings                  |
   */
  SPECIAL_FORM_DECL(LetForm, let)

  /*!
   * @brief Bind values and execute the body only when all bindings are truthy.
   *
   * Usage:
   * @code
   * (if-let [value (lookup key)]
   *   value
   *   :missing)
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | bindings | Vector containing variable name and variable value pairs           |
   * | then     | Form to evaluate when all bound values are truthy                  |
   * | else     | Optional form to evaluate otherwise                                |
   */
  SPECIAL_FORM_DECL(IfLetForm, if_let)

  /*!
   * @brief Temporarily bind values to variables that will exist only
   * within the scope of the body of the when-let macro. The body will execute
   * only if all the bound variables have non-nil values.
   *
   * Usage:
   * @code
   * (when-let [var value] body...)
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | bindings | Vector containing variable name and variable value pairs           |
   * | body...  | Body forms to execute with the temporary bindings                  |
   */
  SPECIAL_FORM_DECL(WhenLetForm, when_let)

  /*!
   * @brief Multi-branch conditional with optional per-clause let bindings.
   *
   * Evaluates test-body pairs in order and returns the body of the first
   * truthy test. When a test is a two-element vector `[sym expr]`, `expr` is
   * evaluated, and if the result is truthy it is bound to `sym` for the
   * duration of that clause's body. When a test is any other form it is
   * evaluated as a plain boolean condition with no binding introduced.
   *
   * Only the first matching branch is evaluated. Returns nil if no test
   * is truthy.
   *
   * Usage:
   * @code
   * (cond-let
   *   [user  (find-user id)]   (greet user)
   *   [guest (find-guest id)]  (welcome guest)
   *   :else                    "unknown visitor")
   * @endcode
   *
   * | Arg          | Description                                                    |
   * | ------------ | -------------------------------------------------------------- |
   * | test body... | Pairs of test (or [sym expr]) and consequent body form.        |
   */
  SPECIAL_FORM_DECL(CondLetForm, cond_let)

} // namespace Roo

#endif /* ROO__LANG__BIND_FORM_H */

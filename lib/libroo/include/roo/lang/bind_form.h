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
   * | Arg # | Description                                                |
   * |-------|------------------------------------------------------------|
   * | 0     | Vector containing a variable name and variable value pairs |
   * | 1...  | Body forms to execute with the temporary bindings          |
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
   * | Arg # | Description                                                |
   * |-------|------------------------------------------------------------|
   * | 0     | Vector containing variable name and variable value pairs   |
   * | 1     | Form to evaluate when all bound values are truthy          |
   * | 2     | Optional form to evaluate otherwise                        |
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
   * | Arg # | Description                                                |
   * |-------|------------------------------------------------------------|
   * | 0     | Vector containing variable name and variable value pairs   |
   * | 1...  | Body forms to execute with the temporary bindings          |
   */
  SPECIAL_FORM_DECL(WhenLetForm, when_let)

} // namespace Roo

#endif /* ROO__LANG__BIND_FORM_H */

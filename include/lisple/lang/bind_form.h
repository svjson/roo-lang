#ifndef LISPLE__LANG__BIND_FORM_H
#define LISPLE__LANG__BIND_FORM_H

#include <lisple/exec.h>

namespace Lisple
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
   * | 0     | Array containing a variable name and variable value pairs  |
   * | 1...  | Body forms to execute with the temporary bindings          |
   */
  SPECIAL_FORM_DECL(LetForm, let)

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
   * | 0     | Array containing variable name and variable value pairs    |
   * | 1...  | Body forms to execute with the temporary bindings          |
   */
  SPECIAL_FORM_DECL(WhenLetForm, when_let)

} // namespace Lisple

#endif /* LISPLE__LANG__BIND_FORM_H */

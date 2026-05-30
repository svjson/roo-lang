
#ifndef ROO__LANG__FUNC_H
#define ROO__LANG__FUNC_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Apply a function or executable form to a seq of arguments.
   *
   * Usage:
   * @code
   * (apply + [1 2 3])
   * => 6
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The function to invoke                                           |
   * | 1     | The seq of arguments                                             |
   */
  FUNC(ApplyFunction, apply)

  /**
   * @brief Define a function in the current namespace
   */
  SPECIAL_FORM_DECL(DefunForm, decl, decl_docstring)

  /*!
   * @brief Define an anonymous lambda function with optional argument
   * destructuring.
   *
   * The body of the function is evaluated when the function is
   * invoked, and the function retains access to the namespace and
   * scope in which it was defined.
   *
   * Usage:
   * @code
   * (fn [arg1 arg2] body...)
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | An vector of argument declarations. Each declaration can be a     |
   * |       | symbol or a destructuring form.                                  |
   * | 1...  | One or more forms that make up the body of the function.         |
   */
  SPECIAL_FORM_DECL(FnForm, decl)

} // namespace Roo

#endif /* ROO__LANG__FUNC_H */

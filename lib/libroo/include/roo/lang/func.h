
#ifndef ROO__LANG__FUNC_H
#define ROO__LANG__FUNC_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Returns its argument unchanged.
   *
   * Usage:
   * @code
   * (identity 42)
   * => 42
   *
   * (map [:a :b :c] identity)
   * => [:a :b :c]
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | x     | Any value                                                          |
   */
  FUNC(IdentityFunction, identity)

  /*!
   * @brief Apply a function or executable form to a seq of arguments.
   *
   * Usage:
   * @code
   * (apply + [1 2 3])
   * => 6
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | f     | The function to invoke                                             |
   * | args  | The seq of arguments                                               |
   */
  FUNC(ApplyFunction, apply)

  /**
   * @brief Define a function in the current namespace
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | name    | Symbol to bind to the function.                                    |
   * | params  | Vector of argument declarations.                                   |
   * | body... | One or more forms that make up the function body.                  |
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
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | params  | An vector of argument declarations. Each declaration can be a      |
   * |         | symbol or a destructuring form.                                    |
   * | body... | One or more forms that make up the body of the function.           |
   */
  SPECIAL_FORM_DECL(FnForm, decl)

} // namespace Roo

#endif /* ROO__LANG__FUNC_H */

#ifndef ROO__LANG__ERR_H
#define ROO__LANG__ERR_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Catch Roo errors and dispatch to the first matching handler clause.
   * @since 0.1.0
   *
   * The guarded body is evaluated normally until it throws a Roo error. The
   * error is exposed to handlers as a map, and clauses are tried in order. The
   * first matching clause evaluates its handler body. If no clause matches,
   * the error is rethrown.
   *
   * Guard clauses have four shapes:
   *
   * | Clause                                  | Matches                                  |
   * | --------------------------------------- | ---------------------------------------- |
   * | `(:selector [error] handler...)`        | The selector.                            |
   * | `(:selector [error test] handler...)`   | The selector, when `test` is truthy.     |
   * | `([error test] handler...)`             | Any Roo error when `test` is truthy.     |
   * | `([error] handler...)`                  | Any Roo error.                           |
   *
   * In each shape, `error` names the local binding for the error map. A
   * qualified selector such as `:roo.io/error` matches that exact error type
   * or a matching parent type. An unqualified selector such as `:roo.io`
   * matches the qualifier of the error type or one of its parent types.
   *
   * Usage:
   * @code
   * (guard
   *   ((:roo.io/error [error] (:message error))
   *    ([error (recoverable? error)] (recover error))
   *    ([error] error))
   *   (risky-operation))
   * @endcode
   *
   * | Arg      | Description                                                   |
   * | -------- | ------------------------------------------------------------- |
   * | clauses  | List of ordered error selector, binding, and handler clauses. |
   * | body...  | Forms evaluated until completion or a Roo error is caught.    |
   *
   * @return The guarded body's result, or the selected handler's result.
   */
  SPECIAL_FORM_DECL(GuardForm, guard)

  /*!
   * @brief Raise a Roo error.
   * @since 0.1.0
   *
   * A string raises a generic `:roo/error`; a qualified keyword supplies the
   * concrete error type; and a map supplies an error directly. The optional
   * metadata map adds flat application-specific facts.
   *
   * Usage:
   * @code
   * (raise "The operation failed")
   * (raise :my-app/not-ready "The account is not ready")
   * (raise :my-app/not-ready "The account is not ready" {:account-id id})
   * (raise error)
   * @endcode
   *
   * | Arg      | Description                                              |
   * | -------- | -------------------------------------------------------- |
   * | error    | Error message, qualified error type, or complete map.    |
   * | message  | Optional message following a qualified error type.       |
   * | metadata | Optional map of flat application-specific error facts.   |
   *
   * @return This function never returns normally.
   */
  FUNC(RaiseFunction, raise)
} // namespace Roo

#endif /* ROO__LANG__ERR_H */

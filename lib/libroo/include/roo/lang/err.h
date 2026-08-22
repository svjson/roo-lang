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
} // namespace Roo

#endif /* ROO__LANG__ERR_H */

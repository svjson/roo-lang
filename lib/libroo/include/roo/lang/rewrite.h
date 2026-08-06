
#ifndef ROO__LANG__REWRITE_H
#define ROO__LANG__REWRITE_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Allows retaining code in a source file without risk of evaluation in
   * runtime.
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | body... | Forms retained without evaluation.                                 |
   */
  SPECIAL_FORM_DECL(CommentForm, comment);

  /*!
   * @brief Thread a value through a sequence of forms as their first argument.
   *
   * Usage:
   * @code
   * (-> value
   *     (first-call)
   *     (second-call))
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | value    | Initial value to thread.                                           |
   * | forms... | Forms to receive the threaded value as first argument.              |
   */
  SPECIAL_FORM_DECL(ThreadFirstForm, thread_first)

  /*!
   * @brief Thread a value through a sequence of forms conditionally.
   *
   * Each step is a test-form pair. The value is threaded as the first argument
   * of a form only when its paired test is truthy; otherwise the value passes
   * through unchanged. Steps are applied in order and each step sees the
   * result of the previous one.
   *
   * Usage:
   * @code
   * (cond-> value
   *   condition1 (assoc :key val)
   *   condition2 (update :other inc))
   * @endcode
   *
   * The form in each pair follows the same threading rules as `->`: if it is a
   * list, the threaded value is inserted as the first argument; if it is a bare
   * symbol or expression, it is called with the threaded value as its only
   * argument.
   *
   * Tests are evaluated in the lexical scope of the `cond->` call - they do not
   * receive the threaded value. When a test is falsy (false or nil), its paired
   * form and all of its arguments are skipped without evaluation.
   *
   * With no test-form pairs the initial value is returned as-is. With no
   * arguments at all, nil is returned.
   *
   * | Arg              | Description                                          |
   * | ---------------- | ---------------------------------------------------- |
   * | value            | Initial value to thread.                             |
   * | test form ...    | Pairs of condition and form to apply when truthy.    |
   */
  SPECIAL_FORM_DECL(CondThreadFirstForm, cond_thread_first)
} // namespace Roo

#endif /* ROO__LANG__REWRITE_H */


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
} // namespace Roo

#endif /* ROO__LANG__REWRITE_H */

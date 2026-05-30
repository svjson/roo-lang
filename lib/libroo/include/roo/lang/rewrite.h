
#ifndef ROO__LANG__REWRITE_H
#define ROO__LANG__REWRITE_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Allows retaining code in a source file without risk of evaluation in
   * runtime.
   */
  SPECIAL_FORM_DECL(CommentForm, comment);

  SPECIAL_FORM_DECL(ThreadFirstForm, thread_first)
} // namespace Roo

#endif /* ROO__LANG__REWRITE_H */

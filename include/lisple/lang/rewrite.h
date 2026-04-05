
#ifndef LISPLE__LANG__REWRITE_H
#define LISPLE__LANG__REWRITE_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Allows retaining code in a source file without risk of evaluation in
   * runtime.
   */
  SPECIAL_FORM_DECL(CommentForm, comment);

  SPECIAL_FORM_DECL(ThreadFirstForm, thread_first)
} // namespace Lisple

#endif /* LISPLE__LANG__REWRITE_H */

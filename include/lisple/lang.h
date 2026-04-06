
#ifndef __LANG_H_
#define __LANG_H_

#include <lisple/exec.h>
#include <lisple/namespace.h>
#include <lisple/type.h>

namespace Lisple
{
  class Context;

  Namespace make_language_namespace();

  FUNC_DECL(PrintFunction, do_print)

  FUNC_DECL(IncludeFunction, include_file)

  /*!
   * @brief Evaluate a Lisple form or a string containing lisple code
   *
   * Usage:
   * @code
   * (eval '(+ 1 1))
   * => 2
   *
   * (eval "(+ 1 1)")
   * => 2
   * @endcode
   */
  FUNC_DECL(EvalFunction, eval_string, eval_form);

  /*!
   * @brief Resolve the value of a symbol programmatically
   *
   * Usage:
   * @code
   * (resolve '+)
   * => <fn>
   * @endcode
   */
  FUNC_DECL(ResolveFunction, resolve);

} // namespace Lisple

#endif

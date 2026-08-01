
#ifndef __LANG_H_
#define __LANG_H_

#include <roo/exec.h>
#include <roo/namespace.h>
#include <roo/type.h>

namespace Roo
{

  /*!
   * @brief Create the immutable native roo language namespace.
   *
   * This namespace contains the built-in functions and special forms that are
   * implicitly available to Roo programs.
   */
  Namespace make_language_namespace();

} // namespace Roo

#endif

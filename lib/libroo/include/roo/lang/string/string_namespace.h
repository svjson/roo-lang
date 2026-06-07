#ifndef ROO__LANG__STRING__STRING_NAMESPACE_H
#define ROO__LANG__STRING__STRING_NAMESPACE_H

#include <roo/namespace.h>

namespace Roo
{
  /*!
   * @brief Create the immutable native roo.string namespace.
   *
   * The namespace contains string-specific functions that are not part of the
   * core language namespace.
   */
  Namespace make_string_namespace();
} // namespace Roo

#endif

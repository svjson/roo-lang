#ifndef ROO__LANG__AST__AST_NAMESPACE_H
#define ROO__LANG__AST__AST_NAMESPACE_H

#include <roo/namespace.h>

namespace Roo
{
  /*!
   * @brief Create the immutable native roo.ast namespace.
   *
   * The namespace contains source reader functions that expose parsed Roo AST
   * nodes as Roo data with source locations.
   */
  Namespace make_ast_namespace();
} // namespace Roo

#endif

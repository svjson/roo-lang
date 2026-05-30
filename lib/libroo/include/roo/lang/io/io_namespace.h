#ifndef ROO__LANG__IO__IO_NAMESPACE_H
#define ROO__LANG__IO__IO_NAMESPACE_H

#include <roo/namespace.h>

namespace Roo
{
  /*!
   * @brief Create the immutable native roo.io namespace.
   *
   * The namespace contains text/EDN file I/O functions, filesystem
   * observation functions, and pure path manipulation functions.
   */
  Namespace make_io_namespace();
  Namespace make_legacy_io_namespace();
} // namespace Roo

#endif

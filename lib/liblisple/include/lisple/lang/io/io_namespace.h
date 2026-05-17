#ifndef LISPLE__LANG__IO__IO_NAMESPACE_H
#define LISPLE__LANG__IO__IO_NAMESPACE_H

#include <lisple/namespace.h>

namespace Lisple
{
  /*!
   * @brief Create the immutable native lisple.io namespace.
   *
   * The namespace contains text/EDN file I/O functions, filesystem
   * observation functions, and pure path manipulation functions.
   */
  Namespace make_io_namespace();
} // namespace Lisple

#endif

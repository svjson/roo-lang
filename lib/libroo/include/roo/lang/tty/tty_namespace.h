#ifndef ROO__LANG__TTY__TTY_NAMESPACE_H
#define ROO__LANG__TTY__TTY_NAMESPACE_H

#include <roo/namespace.h>

namespace Roo
{
  /*!
   * @brief Create the immutable native roo.tty namespace.
   *
   * The namespace contains functions for querying properties of the
   * terminal `stdout` is (or isn't) connected to.
   */
  Namespace make_tty_namespace();
} // namespace Roo

#endif

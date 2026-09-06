#ifndef ROO__RUNTIME__RUNTIME_TRANSFER_H
#define ROO__RUNTIME__RUNTIME_TRANSFER_H

#include <roo/runtime/value.h>

namespace Roo
{
  /** Internal marker for native values whose identity must remain runtime-local. */
  struct RuntimeLocalNativeObject
  {
  };

  /** Copy diagnostic data while representing runtime-owned values as strings. */
  sptr_val deep_copy_for_diagnostic_transfer(const sptr_val& root);
} // namespace Roo

#endif

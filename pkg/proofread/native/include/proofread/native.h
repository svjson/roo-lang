#ifndef PROOFREAD_NATIVE_H
#define PROOFREAD_NATIVE_H

#include <memory>
#include <vector>

#include <roo/namespace.h>

namespace Roo::Proofread
{
  std::unique_ptr<Namespace> make_native_namespace();
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Roo::Proofread

#endif

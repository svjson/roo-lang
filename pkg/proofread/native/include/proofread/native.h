#ifndef PROOFREAD_NATIVE_H
#define PROOFREAD_NATIVE_H

#include <memory>
#include <vector>

#include <lisple/namespace.h>

namespace Lisple::Proofread
{
  std::unique_ptr<Namespace> make_native_namespace();
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Lisple::Proofread

#endif

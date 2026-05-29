#ifndef LOOKUP_NATIVE_H
#define LOOKUP_NATIVE_H

#include <memory>
#include <vector>

#include <lisple/namespace.h>

namespace Lisple::Lookup
{
  std::unique_ptr<Namespace> make_native_namespace();
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Lisple::Lookup

#endif

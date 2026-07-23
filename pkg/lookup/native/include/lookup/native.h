#ifndef LOOKUP_NATIVE_H
#define LOOKUP_NATIVE_H

#include <memory>
#include <vector>

#include <lookup/export.h>
#include <roo/namespace.h>

namespace Roo::Lookup
{
  LOOKUP_NATIVE_API std::unique_ptr<Namespace> make_native_namespace();
  LOOKUP_NATIVE_API std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Roo::Lookup

#endif

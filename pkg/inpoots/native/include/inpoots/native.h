#ifndef INPOOTS_NATIVE_H
#define INPOOTS_NATIVE_H

#include <memory>
#include <vector>

#include <inpoots/export.h>
#include <roo/namespace.h>

namespace Roo::Inpoots
{
  INPOOTS_NATIVE_API std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Roo::Inpoots

#endif /* INPOOTS_NATIVE_H */

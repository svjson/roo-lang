#ifndef ROOPL_NATIVE_H
#define ROOPL_NATIVE_H

#include <memory>
#include <vector>

#include <roo/namespace.h>

#include <roopl/export.h>

namespace Roo::Roopl
{
  ROOPL_NATIVE_API std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Roo::Roopl

#endif /* ROOPL_NATIVE_H */

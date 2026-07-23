#ifndef PROOFREAD_NATIVE_H
#define PROOFREAD_NATIVE_H

#include <memory>
#include <vector>

#include <proofread/export.h>
#include <roo/namespace.h>

namespace Roo::Proofread
{
  PROOFREAD_NATIVE_API std::unique_ptr<Namespace> make_native_namespace();
  PROOFREAD_NATIVE_API std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Roo::Proofread

#endif

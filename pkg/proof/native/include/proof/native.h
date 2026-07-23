#ifndef PROOF_NATIVE_H
#define PROOF_NATIVE_H

#include <memory>
#include <vector>

#include <proof/export.h>
#include <roo/namespace.h>

namespace Roo::Proof
{
  PROOF_NATIVE_API std::unique_ptr<Namespace> make_native_namespace();
  PROOF_NATIVE_API std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Roo::Proof

#endif

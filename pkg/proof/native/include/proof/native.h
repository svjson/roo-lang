#ifndef PROOF_NATIVE_H
#define PROOF_NATIVE_H

#include <memory>
#include <vector>

#include <roo/namespace.h>

namespace Roo::Proof
{
  std::unique_ptr<Namespace> make_native_namespace();
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Roo::Proof

#endif

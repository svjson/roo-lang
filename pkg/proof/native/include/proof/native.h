#ifndef PROOF_NATIVE_H
#define PROOF_NATIVE_H

#include <memory>
#include <vector>

#include <lisple/namespace.h>

namespace Lisple::Proof
{
  std::unique_ptr<Namespace> make_native_namespace();
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces();
} // namespace Lisple::Proof

#endif

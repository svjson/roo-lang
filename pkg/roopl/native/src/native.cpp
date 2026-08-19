#include <binding/target_runtime_namespace.h>
#include <roopl/native.h>

namespace Roo::Roopl
{
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(std::make_unique<TargetRuntimeNamespace>());
    return namespaces;
  }
} // namespace Roo::Roopl

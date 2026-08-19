#include <binding/input_namespace.h>
#include <inpoots/native.h>

namespace Roo::Inpoots
{
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(std::make_unique<InputNamespace>());
    return namespaces;
  }
} // namespace Roo::Inpoots

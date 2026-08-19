#include <proof/native.h>

#include <binding/syntax/fixture_namespace.h>
#include <binding/syntax/scenario_namespace.h>
#include <binding/syntax_namespace.h>

namespace Roo::Proof
{
  std::unique_ptr<Namespace> make_native_namespace()
  {
    return std::make_unique<SyntaxNamespace>();
  }

  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(make_native_namespace());
    namespaces.push_back(std::make_unique<FixtureNamespace>());
    namespaces.push_back(std::make_unique<ScenarioNamespace>());
    return namespaces;
  }
} // namespace Roo::Proof

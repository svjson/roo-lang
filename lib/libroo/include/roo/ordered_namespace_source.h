#ifndef ROO__ORDERED_NAMESPACE_SOURCE_H
#define ROO__ORDERED_NAMESPACE_SOURCE_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <roo/namespace_source.h>

namespace Roo
{
  /**
   * Resolves namespaces from a sequence of sources in priority order.
   */
  class OrderedNamespaceSource : public NamespaceSource, public NamespaceRootConfigurable
  {
    std::vector<std::unique_ptr<NamespaceSource>> sources;

   public:
    explicit OrderedNamespaceSource(std::vector<std::unique_ptr<NamespaceSource>> sources);

    std::optional<NamespaceFetchResult> fetch(
      const std::string& ns_name,
      const NamespaceResolutionContext& ctx) override;

    void set_namespace_roots(std::vector<NamespaceRoot> namespace_roots) override;
  };
} // namespace Roo

#endif

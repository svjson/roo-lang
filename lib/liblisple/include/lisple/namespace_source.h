
#ifndef __LISPLE_NAMESPACE_SOURCE_H_
#define __LISPLE_NAMESPACE_SOURCE_H_

#include <optional>
#include <string>
#include <vector>

namespace Lisple
{
  struct NamespaceRoot
  {
    std::string ns_prefix;
    std::string path;
  };

  struct NamespaceFetchResult
  {
    std::string source;
    std::string resolved_path;
  };

  struct NamespaceResolutionContext
  {
    std::string current_ns_name;
    std::optional<std::string> current_source_path;
  };

  class NamespaceSource
  {
   public:
    virtual ~NamespaceSource() = default;
    virtual std::optional<NamespaceFetchResult> fetch(
      const std::string& ns_name,
      const NamespaceResolutionContext& ctx) = 0;
  };

  class NamespaceRootConfigurable
  {
   public:
    virtual ~NamespaceRootConfigurable() = default;
    virtual void set_namespace_roots(std::vector<NamespaceRoot> namespace_roots) = 0;
  };
} // namespace Lisple

#endif

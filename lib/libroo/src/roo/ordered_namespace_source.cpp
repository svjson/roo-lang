#include "roo/ordered_namespace_source.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <roo/exception.h>
#include <roo/namespace_source.h>

namespace Roo
{
  OrderedNamespaceSource::OrderedNamespaceSource(
    std::vector<std::unique_ptr<NamespaceSource>> sources)
    : sources(std::move(sources))
  {
  }

  std::optional<NamespaceFetchResult> OrderedNamespaceSource::fetch(
    const std::string& ns_name,
    const NamespaceResolutionContext& ctx)
  {
    for (const auto& source : sources)
    {
      if (auto result = source->fetch(ns_name, ctx))
      {
        return result;
      }
    }
    return std::nullopt;
  }

  void OrderedNamespaceSource::set_namespace_roots(
    std::vector<NamespaceRoot> namespace_roots)
  {
    bool configured = false;
    for (const auto& source : sources)
    {
      if (auto* configurable = dynamic_cast<NamespaceRootConfigurable*>(source.get()))
      {
        configurable->set_namespace_roots(namespace_roots);
        configured = true;
      }
    }

    if (!configured)
    {
      throw RooException(
        "Ordered namespace source has no source that supports namespace roots.");
    }
  }
} // namespace Roo

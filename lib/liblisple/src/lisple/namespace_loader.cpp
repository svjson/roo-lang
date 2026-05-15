
#include "lisple/namespace_loader.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <lisple/exception.h>
#include <lisple/namespace.h>
#include <lisple/namespace_source.h>
#include <lisple/runtime.h>

namespace Lisple
{
  NamespaceLoader::NamespaceLoader(NamespaceSource* source)
    : sources({source})
  {
  }

  NamespaceLoader::NamespaceLoader(std::vector<NamespaceSource*> sources)
    : sources(std::move(sources))
  {
  }

  bool NamespaceLoader::is_loading(const std::string& ns_name) const
  {
    return std::find_if(loading_stack.begin(),
                        loading_stack.end(),
                        [&](const LoadingEntry& e)
                        { return e.ns_name == ns_name; }) != loading_stack.end();
  }

  void NamespaceLoader::push_file_context(const std::string& resolved_path)
  {
    loading_stack.push_back({"", resolved_path});
  }

  void NamespaceLoader::pop_file_context()
  {
    loading_stack.pop_back();
  }

  void NamespaceLoader::apply_metadata(Runtime& runtime, const std::string& ns_name)
  {
    if (loading_stack.empty() || loading_stack.back().resolved_path.empty()) return;

    if (auto* ns = runtime.ns(ns_name))
    {
      ns->set_origin(Namespace::Origin::file(loading_stack.back().resolved_path));
    }
  }

  bool NamespaceLoader::load(Runtime& runtime, const std::string& ns_name)
  {
    if (is_loading(ns_name))
    {
      std::string chain;
      for (const auto& entry : loading_stack)
      {
        if (!entry.ns_name.empty())
        {
          chain += entry.ns_name + " -> ";
        }
      }
      chain += ns_name;
      throw CyclicNamespaceException("Cyclic namespace dependency detected: " + chain);
    }

    if (runtime.ns(ns_name) != nullptr)
    {
      return true;
    }

    NamespaceResolutionContext res_ctx{runtime.get_current_namespace().get_name(),
                                       std::nullopt};

    if (!loading_stack.empty())
    {
      res_ctx.current_source_path = loading_stack.back().resolved_path;
    }

    std::optional<NamespaceFetchResult> fetch_result;
    for (auto* s : sources)
    {
      fetch_result = s->fetch(ns_name, res_ctx);
      if (fetch_result) break;
    }

    if (!fetch_result)
    {
      return false;
    }

    loading_stack.push_back({ns_name, fetch_result->resolved_path});
    const std::string current_ns = runtime.get_current_namespace().get_name();
    try
    {
      runtime.eval(fetch_result->source);
    }
    catch (...)
    {
      loading_stack.pop_back();
      runtime.switch_namespace(current_ns);
      throw;
    }
    loading_stack.pop_back();
    runtime.switch_namespace(current_ns);

    if (auto* loaded_ns = runtime.ns(ns_name))
    {
      loaded_ns->set_origin(Namespace::Origin::file(fetch_result->resolved_path));
    }

    return true;
  }
} // namespace Lisple


#include "roo/namespace_loader.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <roo/exception.h>
#include <roo/namespace.h>
#include <roo/namespace_source.h>
#include <roo/runtime.h>

namespace Roo
{
  namespace
  {
    bool has_parse_file_context(const Roo::ParseException& e)
    {
      const std::string message = e.what();
      return message.compare(0, 15, "Error parsing '") == 0;
    }
  } // namespace

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

    if (auto* existing_ns = runtime.ns(ns_name, false))
    {
      if (existing_ns->get_origin().type == Namespace::Origin::Type::FILE ||
          !existing_ns->empty())
      {
        return true;
      }
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
    if (auto* ns = runtime.ns(ns_name, true))
    {
      ns->set_origin(Namespace::Origin::file(fetch_result->resolved_path));
    }

    try
    {
      runtime.eval(fetch_result->source, fetch_result->resolved_path);
    }
    catch (const Roo::ParseException& e)
    {
      loading_stack.pop_back();
      runtime.switch_namespace(current_ns);
      if (has_parse_file_context(e))
      {
        throw;
      }
      throw Roo::ParseException("Error parsing '" + fetch_result->resolved_path +
                                "': " + e.what());
    }
    catch (...)
    {
      loading_stack.pop_back();
      runtime.switch_namespace(current_ns);
      throw;
    }
    loading_stack.pop_back();
    if (auto* loaded_ns = runtime.ns(ns_name))
    {
      loaded_ns->set_origin(Namespace::Origin::file(fetch_result->resolved_path));
    }
    runtime.switch_namespace(current_ns);

    return true;
  }
} // namespace Roo

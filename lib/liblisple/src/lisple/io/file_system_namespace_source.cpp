
#include "lisple/io/file_system_namespace_source.h"

#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <lisple/exception.h>
#include <lisple/io/file_system.h>
#include <lisple/namespace_source.h>

namespace Lisple
{
  namespace
  {
    std::vector<std::string> split(const std::string& value, char delimiter)
    {
      std::vector<std::string> parts;
      size_t start = 0;

      while (start <= value.size())
      {
        auto pos = value.find(delimiter, start);
        if (pos == std::string::npos)
        {
          auto part = value.substr(start);
          if (!part.empty())
          {
            parts.push_back(part);
          }
          break;
        }

        auto part = value.substr(start, pos - start);
        if (!part.empty())
        {
          parts.push_back(part);
        }
        start = pos + 1;
      }

      return parts;
    }

    std::string join(const std::vector<std::string>& parts, char delimiter)
    {
      std::string joined;

      for (size_t i = 0; i < parts.size(); ++i)
      {
        if (i > 0)
        {
          joined += delimiter;
        }
        joined += parts[i];
      }

      return joined;
    }

    std::string strip_extension(const std::string& file_name)
    {
      auto last_dot = file_name.find_last_of('.');
      if (last_dot == std::string::npos)
      {
        return file_name;
      }
      return file_name.substr(0, last_dot);
    }

    std::string normalize_path(const std::vector<std::string>& parts)
    {
      std::vector<std::string> normalized;

      for (const auto& part : parts)
      {
        if (part == ".")
        {
          continue;
        }
        if (part == "..")
        {
          if (!normalized.empty() && normalized.back() != "..")
          {
            normalized.pop_back();
          }
          else
          {
            normalized.push_back(part);
          }
          continue;
        }
        normalized.push_back(part);
      }

      return join(normalized, '/');
    }

    std::string join_path(const std::string& root, const std::string& child)
    {
      if (root.empty() || root == ".")
      {
        return child;
      }
      if (child.empty())
      {
        return root;
      }
      if (root.back() == '/')
      {
        return root + child;
      }
      return root + "/" + child;
    }

    bool namespace_root_matches(const std::string& ns_name,
                                const std::string& ns_prefix)
    {
      return ns_name == ns_prefix ||
             (ns_name.size() > ns_prefix.size() &&
              ns_name.compare(0, ns_prefix.size(), ns_prefix) == 0 &&
              ns_name[ns_prefix.size()] == '.');
    }
  } // namespace

  FileSystemNamespaceSource::FileSystemNamespaceSource(FileSystem* fs)
    : FileSystemNamespaceSource(fs, {".lisple", ".lspl"})
  {
  }

  FileSystemNamespaceSource::FileSystemNamespaceSource(FileSystem* fs,
                                                       std::vector<std::string> extensions)
    : FileSystemNamespaceSource(fs, std::move(extensions), {})
  {
  }

  FileSystemNamespaceSource::FileSystemNamespaceSource(
    FileSystem* fs,
    std::vector<std::string> extensions,
    std::vector<NamespaceRoot> namespace_roots)
    : fs(fs)
    , extensions(std::move(extensions))
    , namespace_roots(std::move(namespace_roots))
  {
  }

  void FileSystemNamespaceSource::set_namespace_roots(
    std::vector<NamespaceRoot> namespace_roots)
  {
    this->namespace_roots = std::move(namespace_roots);
  }

  std::string FileSystemNamespaceSource::ns_to_path(const std::string& ns_name) const
  {
    std::string path = ns_name;
    std::replace(path.begin(), path.end(), '.', '/');
    return path;
  }

  std::vector<std::string> FileSystemNamespaceSource::namespace_root_paths(
    const std::string& ns_name) const
  {
    std::vector<const NamespaceRoot*> matches;
    for (const auto& root : namespace_roots)
    {
      if (!root.ns_prefix.empty() && namespace_root_matches(ns_name, root.ns_prefix))
      {
        matches.push_back(&root);
      }
    }

    std::stable_sort(matches.begin(),
                     matches.end(),
                     [](const NamespaceRoot* a, const NamespaceRoot* b)
                     { return a->ns_prefix.size() > b->ns_prefix.size(); });

    std::vector<std::string> paths;
    paths.reserve(matches.size());
    for (const auto* root : matches)
    {
      std::string suffix;
      if (ns_name.size() > root->ns_prefix.size())
      {
        suffix = ns_name.substr(root->ns_prefix.size() + 1);
        std::replace(suffix.begin(), suffix.end(), '.', '/');
      }
      paths.push_back(join_path(root->path, suffix));
    }
    return paths;
  }

  std::string FileSystemNamespaceSource::infer_path(
    const std::string& ns_name,
    const std::string& current_ns_name,
    const std::string& current_source_path) const
  {
    const std::vector<std::string> current_ns_segments = split(current_ns_name, '.');
    const std::vector<std::string> target_ns_segments = split(ns_name, '.');
    if (current_ns_segments.empty() || target_ns_segments.empty())
    {
      return {};
    }

    const auto last_slash = current_source_path.find_last_of('/');
    const std::string dir_path =
      last_slash == std::string::npos ? "" : current_source_path.substr(0, last_slash);
    const std::string file_name = last_slash == std::string::npos
                                    ? current_source_path
                                    : current_source_path.substr(last_slash + 1);
    const std::string file_stem = strip_extension(file_name);
    const std::vector<std::string> dir_segments = split(dir_path, '/');

    size_t common_prefix = 0;
    while (common_prefix < current_ns_segments.size() &&
           common_prefix < target_ns_segments.size() &&
           current_ns_segments[common_prefix] == target_ns_segments[common_prefix])
    {
      common_prefix++;
    }

    if (common_prefix == 0 || target_ns_segments.size() <= common_prefix)
    {
      return {};
    }

    const bool file_matches_leaf =
      !file_stem.empty() && file_stem == current_ns_segments.back();
    std::vector<std::string> resolved_parts = dir_segments;

    if (common_prefix < current_ns_segments.size())
    {
      const size_t parent_hops = current_ns_segments.size() - 1 - common_prefix;
      for (size_t i = 0; i < parent_hops; ++i)
      {
        resolved_parts.push_back("..");
      }
    }
    else if (file_matches_leaf)
    {
      resolved_parts.push_back(file_stem);
    }

    resolved_parts.insert(resolved_parts.end(),
                          target_ns_segments.begin() + static_cast<long>(common_prefix),
                          target_ns_segments.end());

    return normalize_path(resolved_parts);
  }

  std::optional<NamespaceFetchResult> FileSystemNamespaceSource::fetch(
    const std::string& ns_name,
    const NamespaceResolutionContext& ctx)
  {
    for (const auto& rooted_path : namespace_root_paths(ns_name))
    {
      for (const auto& ext : extensions)
      {
        try
        {
          auto source = fs->read(rooted_path + ext);
          return NamespaceFetchResult{std::move(source), rooted_path + ext};
        }
        catch (const LispleException&)
        {
        }
      }
    }

    if (ctx.current_source_path)
    {
      const std::string inferred_path =
        infer_path(ns_name, ctx.current_ns_name, *ctx.current_source_path);

      if (!inferred_path.empty())
      {
        for (const auto& ext : extensions)
        {
          try
          {
            auto source = fs->read(inferred_path + ext);
            return NamespaceFetchResult{std::move(source), inferred_path + ext};
          }
          catch (const LispleException&)
          {
          }
        }
      }
    }

    const std::string full_path = ns_to_path(ns_name);
    for (const auto& ext : extensions)
    {
      try
      {
        auto source = fs->read(full_path + ext);
        return NamespaceFetchResult{std::move(source), full_path + ext};
      }
      catch (const LispleException&)
      {
      }
    }
    return std::nullopt;
  }
} // namespace Lisple


#include "lisple/file_system_namespace_source.h"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include <lisple/exception.h>
#include <lisple/file_system.h>
#include <lisple/namespace_source.h>

namespace Lisple
{
  FileSystemNamespaceSource::FileSystemNamespaceSource(FileSystem* fs)
    : FileSystemNamespaceSource(fs, {".lisple", ".lspl"})
  {
  }

  FileSystemNamespaceSource::FileSystemNamespaceSource(FileSystem* fs,
                                                       std::vector<std::string> extensions)
    : fs(fs)
    , extensions(std::move(extensions))
  {
  }

  std::string FileSystemNamespaceSource::ns_to_path(const std::string& ns_name) const
  {
    std::string path = ns_name;
    std::replace(path.begin(), path.end(), '.', '/');
    return path;
  }

  std::string FileSystemNamespaceSource::infer_path(
    const std::string& ns_name,
    const std::string& current_ns_name,
    const std::string& current_source_path) const
  {
    auto ns_dot = ns_name.find('.');
    auto cur_dot = current_ns_name.find('.');
    if (ns_dot == std::string::npos || cur_dot == std::string::npos)
    {
      return {};
    }
    if (ns_name.substr(0, ns_dot) != current_ns_name.substr(0, cur_dot))
    {
      return {};
    }

    const size_t cur_segments =
      1 +
      static_cast<size_t>(std::count(current_ns_name.begin(), current_ns_name.end(), '.'));
    const size_t dir_depth = static_cast<size_t>(
      std::count(current_source_path.begin(), current_source_path.end(), '/'));

    if (cur_segments - 1 > dir_depth)
    {
      /**
       * Flat case: more namespace segments than path directories.
       * Strip the segments not reflected in the filesystem structure.
       */
      const size_t strip_count = (cur_segments - 1) - dir_depth;
      std::string remaining = ns_name;
      for (size_t i = 0; i < strip_count; ++i)
      {
        auto dot = remaining.find('.');
        if (dot == std::string::npos)
        {
          return {};
        }
        remaining = remaining.substr(dot + 1);
      }
      if (remaining.empty())
      {
        return {};
      }
      return ns_to_path(remaining);
    }

    /**
     * Rooted case: path depth covers the full namespace depth.
     * The leading (dir_depth - cur_segments + 2) path components are the
     * package root directory; the required namespace is resolved under it.
     */
    const size_t root_dir_depth = dir_depth - cur_segments + 2;
    std::string root_dir;
    std::string path_tail = current_source_path;
    for (size_t i = 0; i < root_dir_depth; ++i)
    {
      auto slash = path_tail.find('/');
      if (slash == std::string::npos)
      {
        return {};
      }
      root_dir += path_tail.substr(0, slash + 1);
      path_tail = path_tail.substr(slash + 1);
    }

    auto dot = ns_name.find('.');
    if (dot == std::string::npos)
    {
      return {};
    }
    std::string ns_tail = ns_name.substr(dot + 1);
    if (ns_tail.empty())
    {
      return {};
    }
    return root_dir + ns_to_path(ns_tail);
  }

  std::optional<NamespaceFetchResult> FileSystemNamespaceSource::fetch(
    const std::string& ns_name,
    const NamespaceResolutionContext& ctx)
  {
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
            auto source = fs->read_file_to_string(inferred_path + ext);
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
        auto source = fs->read_file_to_string(full_path + ext);
        return NamespaceFetchResult{std::move(source), full_path + ext};
      }
      catch (const LispleException&)
      {
      }
    }
    return std::nullopt;
  }
} // namespace Lisple

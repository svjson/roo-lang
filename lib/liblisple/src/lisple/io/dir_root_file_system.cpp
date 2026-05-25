#include "lisple/io/dir_root_file_system.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

#include <lisple/exception.h>

namespace Lisple
{
  namespace
  {
    FileSystemEntryType entry_type(const std::filesystem::file_status& status)
    {
      if (std::filesystem::is_regular_file(status))
      {
        return FileSystemEntryType::FILE;
      }
      if (std::filesystem::is_directory(status))
      {
        return FileSystemEntryType::DIRECTORY;
      }
      return FileSystemEntryType::OTHER;
    }

    long modified_ms(const std::filesystem::path& path)
    {
      std::error_code ec;
      using namespace std::chrono;
      const auto file_time = std::filesystem::last_write_time(path, ec);
      if (ec) return 0;
      const auto system_time = time_point_cast<milliseconds>(
        file_time - std::filesystem::file_time_type::clock::now() + system_clock::now());
      return system_time.time_since_epoch().count();
    }

    bool hidden_name(const std::filesystem::path& path)
    {
      const std::string name = path.filename().string();
      return !name.empty() && name[0] == '.';
    }

    std::string resolve_path(const std::string& load_path, const std::string& file_name)
    {
      return (std::filesystem::path(load_path) / file_name).lexically_normal().string();
    }

    std::filesystem::path readable_path(const std::vector<std::string>& load_paths,
                                        const std::string& path)
    {
      const std::filesystem::path requested(path);
      if (requested.is_absolute())
      {
        return requested.lexically_normal();
      }

      for (const std::string& load_path : load_paths)
      {
        const std::filesystem::path candidate = resolve_path(load_path, path);
        std::error_code ec;
        if (std::filesystem::is_regular_file(candidate, ec))
        {
          return candidate;
        }
      }
      return std::filesystem::path(
        resolve_path(load_paths.empty() ? "." : load_paths.front(), path));
    }

    std::filesystem::path writable_path(const std::vector<std::string>& load_paths,
                                        const std::string& path)
    {
      const std::filesystem::path requested(path);
      if (requested.is_absolute())
      {
        return requested.lexically_normal();
      }
      const std::string root = load_paths.empty() ? "." : load_paths.front();
      return std::filesystem::path(resolve_path(root, path));
    }

    std::string read_file(const std::string& path)
    {
      std::ifstream stream(path);
      if (stream.fail())
      {
        stream.close();
        throw LispleException("File not found on load path: '" + path + "'");
      }
      std::stringstream buffer;
      buffer << stream.rdbuf();
      stream.close();
      return buffer.str();
    }

    FileSystemStat stat_path(const std::filesystem::path& path)
    {
      FileSystemStat result;
      std::error_code ec;
      const std::filesystem::file_status status = std::filesystem::status(path, ec);
      if (ec || !std::filesystem::exists(status))
      {
        return result;
      }

      result.exists = true;
      result.type = entry_type(status);
      if (result.type == FileSystemEntryType::FILE)
      {
        result.size = std::filesystem::file_size(path, ec);
        if (ec) result.size = 0;
      }
      result.modified_ms = modified_ms(path);
      return result;
    }

    std::string getenv_or_empty(const char* name)
    {
      if (const char* value = std::getenv(name))
      {
        return value;
      }
      return "";
    }
  } // namespace

  DirRootFileSystem::DirRootFileSystem(const std::string& dir_root)
    : DirRootFileSystem(std::vector<std::string>{dir_root})
  {
  }

  DirRootFileSystem::DirRootFileSystem(const std::vector<std::string>& load_paths)
    : load_paths(load_paths)
  {
  }

  const std::string DirRootFileSystem::read(const std::string& file_name)
  {
    for (auto& load_path : load_paths)
    {
      const std::string relative_path = resolve_path(load_path, file_name);
      try
      {
        return read_file(relative_path);
      }
      catch (const LispleException&)
      {
      }
    }
    throw LispleException("File not found on load path: '" + file_name + "'");
  }

  void DirRootFileSystem::write(const std::string& file_name, const std::string& contents)
  {
    const std::filesystem::path path = writable_path(load_paths, file_name);
    const auto parent_path = path.parent_path();
    if (!parent_path.empty())
    {
      std::filesystem::create_directories(parent_path);
    }

    std::ofstream stream(path);
    if (stream.fail())
    {
      stream.close();
      throw LispleException("Could not write file: '" + path.string() + "'");
    }
    stream << contents;
    stream.close();
  }

  void DirRootFileSystem::copy_file(const std::string& source,
                                    const std::string& destination)
  {
    const std::filesystem::path source_path = readable_path(load_paths, source);
    const std::filesystem::path destination_path = writable_path(load_paths, destination);
    const auto parent_path = destination_path.parent_path();
    if (!parent_path.empty())
    {
      std::filesystem::create_directories(parent_path);
    }

    std::error_code ec;
    std::filesystem::copy_file(source_path,
                               destination_path,
                               std::filesystem::copy_options::overwrite_existing,
                               ec);
    if (ec)
    {
      throw LispleException("Could not copy file: '" + source_path.string() + "' -> '" +
                            destination_path.string() + "': " + ec.message());
    }
  }

  void DirRootFileSystem::remove_tree(const std::string& path)
  {
    const std::filesystem::path target = writable_path(load_paths, path);
    std::error_code ec;
    std::filesystem::remove_all(target, ec);
    if (ec)
    {
      throw LispleException("Could not remove path: '" + target.string() +
                            "': " + ec.message());
    }
  }

  void DirRootFileSystem::create_symlink(const std::string& source, const std::string& link)
  {
    const std::filesystem::path source_path = writable_path(load_paths, source);
    const std::filesystem::path link_path = writable_path(load_paths, link);
    const auto parent_path = link_path.parent_path();
    if (!parent_path.empty())
    {
      std::filesystem::create_directories(parent_path);
    }

    std::error_code ec;
    if (std::filesystem::is_directory(source_path, ec))
    {
      std::filesystem::create_directory_symlink(source_path, link_path, ec);
    }
    else
    {
      std::filesystem::create_symlink(source_path, link_path, ec);
    }
    if (ec)
    {
      throw LispleException("Could not create symlink: '" + source_path.string() + "' -> '" +
                            link_path.string() + "': " + ec.message());
    }
  }

  bool DirRootFileSystem::is_symlink(const std::string& path)
  {
    const std::filesystem::path target = writable_path(load_paths, path);
    std::error_code ec;
    return std::filesystem::is_symlink(std::filesystem::symlink_status(target, ec));
  }

  std::string DirRootFileSystem::read_symlink(const std::string& path)
  {
    const std::filesystem::path target = writable_path(load_paths, path);
    std::error_code ec;
    const std::filesystem::path link_target = std::filesystem::read_symlink(target, ec);
    if (ec)
    {
      throw LispleException("Could not read symlink: '" + target.string() +
                            "': " + ec.message());
    }
    return link_target.lexically_normal().string();
  }

  FileSystemStat DirRootFileSystem::stat(const std::string& path)
  {
    const std::filesystem::path requested(path);
    if (requested.is_absolute())
    {
      return stat_path(requested);
    }

    for (const std::string& load_path : load_paths)
    {
      FileSystemStat result = stat_path(resolve_path(load_path, path));
      if (result.exists)
      {
        return result;
      }
    }
    return stat_path(resolve_path(load_paths.empty() ? "." : load_paths.front(), path));
  }

  bool DirRootFileSystem::exists(const std::string& path)
  {
    return stat(path).exists;
  }

  bool DirRootFileSystem::is_file(const std::string& path)
  {
    FileSystemStat file_stat = stat(path);
    return file_stat.exists && file_stat.type == FileSystemEntryType::FILE;
  }

  bool DirRootFileSystem::is_directory(const std::string& path)
  {
    FileSystemStat file_stat = stat(path);
    return file_stat.exists && file_stat.type == FileSystemEntryType::DIRECTORY;
  }

  std::vector<DirectoryEntry> DirRootFileSystem::list_directory(const std::string& path)
  {
    std::filesystem::path directory;
    const std::filesystem::path requested(path);
    if (requested.is_absolute())
    {
      directory = requested;
    }
    else
    {
      for (const std::string& load_path : load_paths)
      {
        const std::filesystem::path candidate = resolve_path(load_path, path);
        std::error_code ec;
        if (std::filesystem::is_directory(candidate, ec))
        {
          directory = candidate;
          break;
        }
      }
      if (directory.empty())
      {
        directory = resolve_path(load_paths.empty() ? "." : load_paths.front(), path);
      }
    }

    std::error_code ec;
    if (!std::filesystem::is_directory(directory, ec))
    {
      throw LispleException("Directory not found on load path: '" + path + "'");
    }

    std::vector<DirectoryEntry> entries;
    for (const std::filesystem::directory_entry& entry :
         std::filesystem::directory_iterator(directory))
    {
      DirectoryEntry result;
      result.name = entry.path().filename().string();
      result.path = (std::filesystem::path(path) / result.name).lexically_normal().string();
      result.type = entry_type(entry.status());
      result.hidden = hidden_name(entry.path());
      entries.push_back(result);
    }

    std::sort(entries.begin(),
              entries.end(),
              [](const DirectoryEntry& a, const DirectoryEntry& b)
              { return a.name < b.name; });
    return entries;
  }

  std::string DirRootFileSystem::absolute_path(const std::string& path)
  {
    const std::filesystem::path requested(path);
    if (requested.is_absolute())
    {
      return requested.lexically_normal().string();
    }

    const std::string root = load_paths.empty() ? "." : load_paths.front();
    return std::filesystem::absolute(std::filesystem::path(root) / requested)
      .lexically_normal()
      .string();
  }

  std::string DirRootFileSystem::current_directory()
  {
    return std::filesystem::current_path().string();
  }

  std::string DirRootFileSystem::home_directory()
  {
    std::string home = getenv_or_empty("HOME");
    if (home.empty())
    {
      home = getenv_or_empty("USERPROFILE");
    }
    if (home.empty())
    {
      throw LispleException("Could not determine home directory");
    }
    return home;
  }

} // namespace Lisple

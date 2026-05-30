#include "fake_file_system.h"

#include <algorithm>
#include <filesystem>

namespace RooTest
{
  namespace
  {
    std::string normalize(const std::string& path)
    {
      return std::filesystem::path(path).lexically_normal().string();
    }

    bool hidden_name(const std::string& name)
    {
      return !name.empty() && name[0] == '.';
    }
  } // namespace

  const std::string FakeFileSystem::read(const std::string& file_name)
  {
    const std::string normalized_file_name = normalize(file_name);
    if (fs_symlinks.count(normalized_file_name))
    {
      return fs_contents.at(normalize(fs_symlinks.at(normalized_file_name)));
    }
    return fs_contents.at(normalized_file_name);
  }

  void FakeFileSystem::add_file(const std::string& file_name,
                                const std::string& file_contents)
  {
    fs_contents.emplace(normalize(file_name), file_contents);
  }

  bool FakeFileSystem::has_file(const std::string& file_name) const
  {
    return fs_contents.count(normalize(file_name)) > 0;
  }

  const std::string& FakeFileSystem::get_file(const std::string& file_name) const
  {
    return fs_contents.at(normalize(file_name));
  }

  void FakeFileSystem::write(const std::string& file_name, const std::string& contents)
  {
    fs_contents[normalize(file_name)] = contents;
  }

  void FakeFileSystem::copy_file(const std::string& source, const std::string& destination)
  {
    fs_contents[normalize(destination)] = fs_contents.at(normalize(source));
  }

  void FakeFileSystem::remove_tree(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
    if (fs_symlinks.erase(normalized_path) > 0)
    {
      return;
    }
    for (auto it = fs_contents.begin(); it != fs_contents.end();)
    {
      if (it->first == normalized_path || it->first.rfind(normalized_path + "/", 0) == 0)
      {
        it = fs_contents.erase(it);
      }
      else
      {
        ++it;
      }
    }
    for (auto it = fs_symlinks.begin(); it != fs_symlinks.end();)
    {
      if (it->first.rfind(normalized_path + "/", 0) == 0)
      {
        it = fs_symlinks.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  void FakeFileSystem::create_symlink(const std::string& source, const std::string& link)
  {
    fs_symlinks[normalize(link)] = normalize(source);
  }

  bool FakeFileSystem::is_symlink(const std::string& path)
  {
    return fs_symlinks.count(normalize(path)) > 0;
  }

  std::string FakeFileSystem::read_symlink(const std::string& path)
  {
    return fs_symlinks.at(normalize(path));
  }

  std::vector<Roo::DirectoryEntry> FakeFileSystem::list_directory(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
    std::map<std::string, Roo::DirectoryEntry> entries;

    for (const auto& [file_path, contents] : fs_contents)
    {
      (void)contents;

      std::filesystem::path relative;
      if (normalized_path == "." || normalized_path.empty())
      {
        relative = file_path;
      }
      else if (file_path.rfind(normalized_path + "/", 0) == 0)
      {
        relative = file_path.substr(normalized_path.size() + 1);
      }
      else
      {
        continue;
      }

      if (relative.empty()) continue;
      auto child = relative.begin();
      const std::string name = child->string();
      const bool directory = ++child != relative.end();

      Roo::DirectoryEntry entry;
      entry.name = name;
      entry.path = normalize((std::filesystem::path(normalized_path) / name).string());
      entry.type =
        directory ? Roo::FileSystemEntryType::DIRECTORY : Roo::FileSystemEntryType::FILE;
      entry.hidden = hidden_name(name);
      entries[name] = entry;
    }
    for (const auto& [link_path, target] : fs_symlinks)
    {
      (void)target;

      std::filesystem::path relative;
      if (normalized_path == "." || normalized_path.empty())
      {
        relative = link_path;
      }
      else if (link_path.rfind(normalized_path + "/", 0) == 0)
      {
        relative = link_path.substr(normalized_path.size() + 1);
      }
      else
      {
        continue;
      }

      if (relative.empty()) continue;
      auto child = relative.begin();
      const std::string name = child->string();
      const bool directory = ++child != relative.end();

      Roo::DirectoryEntry entry;
      entry.name = name;
      entry.path = normalize((std::filesystem::path(normalized_path) / name).string());
      entry.type =
        directory ? Roo::FileSystemEntryType::DIRECTORY : Roo::FileSystemEntryType::OTHER;
      entry.hidden = hidden_name(name);
      entries[name] = entry;
    }

    std::vector<Roo::DirectoryEntry> result;
    for (const auto& [name, entry] : entries)
    {
      (void)name;
      result.push_back(entry);
    }
    return result;
  }

  Roo::FileSystemStat FakeFileSystem::stat(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
    Roo::FileSystemStat result;
    if (fs_symlinks.count(normalized_path))
    {
      return stat(fs_symlinks.at(normalized_path));
    }
    if (has_file(normalized_path))
    {
      result.exists = true;
      result.type = Roo::FileSystemEntryType::FILE;
      result.size = fs_contents.at(normalized_path).size();
      return result;
    }
    if (is_directory(normalized_path))
    {
      result.exists = true;
      result.type = Roo::FileSystemEntryType::DIRECTORY;
    }
    return result;
  }

  bool FakeFileSystem::exists(const std::string& path)
  {
    return is_symlink(path) || has_file(path) || is_directory(path);
  }

  bool FakeFileSystem::is_file(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
    if (fs_symlinks.count(normalized_path))
    {
      return is_file(fs_symlinks.at(normalized_path));
    }
    return has_file(normalized_path);
  }

  bool FakeFileSystem::is_directory(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
    if (fs_symlinks.count(normalized_path))
    {
      return is_directory(fs_symlinks.at(normalized_path));
    }
    if (normalized_path == "." || normalized_path.empty())
    {
      return true;
    }
    return std::any_of(fs_contents.begin(),
                       fs_contents.end(),
                       [&](const auto& entry)
                       { return entry.first.rfind(normalized_path + "/", 0) == 0; });
  }

  std::string FakeFileSystem::absolute_path(const std::string& path)
  {
    return normalize((std::filesystem::path("/fake") / path).string());
  }

  std::string FakeFileSystem::current_directory()
  {
    return "/fake";
  }

  std::string FakeFileSystem::home_directory()
  {
    return "/home/fake";
  }
} // namespace RooTest

#include "fake_file_system.h"

#include <algorithm>
#include <filesystem>

namespace LispleTest
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
    return fs_contents.at(normalize(file_name));
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

  std::vector<Lisple::DirectoryEntry> FakeFileSystem::list_directory(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
    std::map<std::string, Lisple::DirectoryEntry> entries;

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

      Lisple::DirectoryEntry entry;
      entry.name = name;
      entry.path = normalize((std::filesystem::path(normalized_path) / name).string());
      entry.type = directory ? Lisple::FileSystemEntryType::DIRECTORY
                             : Lisple::FileSystemEntryType::FILE;
      entry.hidden = hidden_name(name);
      entries[name] = entry;
    }

    std::vector<Lisple::DirectoryEntry> result;
    for (const auto& [name, entry] : entries)
    {
      (void)name;
      result.push_back(entry);
    }
    return result;
  }

  Lisple::FileSystemStat FakeFileSystem::stat(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
    Lisple::FileSystemStat result;
    if (has_file(normalized_path))
    {
      result.exists = true;
      result.type = Lisple::FileSystemEntryType::FILE;
      result.size = fs_contents.at(normalized_path).size();
      return result;
    }
    if (is_directory(normalized_path))
    {
      result.exists = true;
      result.type = Lisple::FileSystemEntryType::DIRECTORY;
    }
    return result;
  }

  bool FakeFileSystem::exists(const std::string& path)
  {
    return has_file(path) || is_directory(path);
  }

  bool FakeFileSystem::is_file(const std::string& path)
  {
    return has_file(path);
  }

  bool FakeFileSystem::is_directory(const std::string& path)
  {
    const std::string normalized_path = normalize(path);
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
} // namespace LispleTest

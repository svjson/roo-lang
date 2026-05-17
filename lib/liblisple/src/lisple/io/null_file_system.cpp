#include "lisple/io/null_file_system.h"

#include <lisple/exception.h>

namespace Lisple
{
  namespace
  {
    LispleException no_file_system_access(const std::string& operation,
                                          const std::string& path = "")
    {
      if (path.empty())
      {
        return LispleException(
          "This Lisple context does not provide any file system access for " + operation);
      }
      return LispleException(
        "This Lisple context does not provide any file system access for " + operation +
        ": '" + path + "'");
    }
  } // namespace

  const std::string NullFileSystem::read(const std::string& file_name)
  {
    throw no_file_system_access("read", file_name);
  }

  void NullFileSystem::write(const std::string& file_name, const std::string&)
  {
    throw no_file_system_access("write", file_name);
  }

  std::vector<DirectoryEntry> NullFileSystem::list_directory(const std::string& path)
  {
    throw no_file_system_access("list-directory", path);
  }

  FileSystemStat NullFileSystem::stat(const std::string& path)
  {
    throw no_file_system_access("stat", path);
  }

  bool NullFileSystem::exists(const std::string& path)
  {
    throw no_file_system_access("exists", path);
  }

  bool NullFileSystem::is_file(const std::string& path)
  {
    throw no_file_system_access("file?", path);
  }

  bool NullFileSystem::is_directory(const std::string& path)
  {
    throw no_file_system_access("directory?", path);
  }

  std::string NullFileSystem::absolute_path(const std::string& path)
  {
    throw no_file_system_access("absolute-path", path);
  }

  std::string NullFileSystem::current_directory()
  {
    throw no_file_system_access("current-directory");
  }

  std::string NullFileSystem::home_directory()
  {
    throw no_file_system_access("home-directory");
  }
} // namespace Lisple

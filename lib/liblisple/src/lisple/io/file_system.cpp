#include "lisple/io/file_system.h"

#include <lisple/exception.h>

namespace Lisple
{
  void FileSystem::write(const std::string& file_name, const std::string&)
  {
    throw LispleException("File system does not support writing: '" + file_name + "'");
  }

  void FileSystem::copy_file(const std::string& source, const std::string& destination)
  {
    throw LispleException("File system does not support copying files: '" + source +
                          "' -> '" + destination + "'");
  }

  void FileSystem::remove_tree(const std::string& path)
  {
    throw LispleException("File system does not support removing paths: '" + path + "'");
  }

  void FileSystem::create_symlink(const std::string& source, const std::string& link)
  {
    throw LispleException("File system does not support creating symlinks: '" + source +
                          "' -> '" + link + "'");
  }

  bool FileSystem::is_symlink(const std::string& path)
  {
    throw LispleException("File system does not support symlink checks: '" + path + "'");
  }

  std::string FileSystem::read_symlink(const std::string& path)
  {
    throw LispleException("File system does not support reading symlinks: '" + path + "'");
  }

  std::vector<DirectoryEntry> FileSystem::list_directory(const std::string& path)
  {
    throw LispleException("File system does not support listing directories: '" + path +
                          "'");
  }

  FileSystemStat FileSystem::stat(const std::string& path)
  {
    throw LispleException("File system does not support stat: '" + path + "'");
  }

  bool FileSystem::exists(const std::string& path)
  {
    return stat(path).exists;
  }

  bool FileSystem::is_file(const std::string& path)
  {
    FileSystemStat file_stat = stat(path);
    return file_stat.exists && file_stat.type == FileSystemEntryType::FILE;
  }

  bool FileSystem::is_directory(const std::string& path)
  {
    FileSystemStat file_stat = stat(path);
    return file_stat.exists && file_stat.type == FileSystemEntryType::DIRECTORY;
  }

  std::string FileSystem::absolute_path(const std::string& path)
  {
    throw LispleException("File system does not support absolute paths: '" + path + "'");
  }

  std::string FileSystem::current_directory()
  {
    throw LispleException("File system does not support current directory lookup");
  }

  std::string FileSystem::home_directory()
  {
    throw LispleException("File system does not support home directory lookup");
  }
} // namespace Lisple

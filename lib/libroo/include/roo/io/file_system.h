#ifndef ROO__IO__FILE_SYSTEM_H
#define ROO__IO__FILE_SYSTEM_H

#include <cstdint>
#include <string>
#include <vector>

namespace Roo
{
  enum class FileSystemEntryType
  {
    FILE,
    DIRECTORY,
    OTHER
  };

  struct FileSystemStat
  {
    bool exists = false;
    FileSystemEntryType type = FileSystemEntryType::OTHER;
    std::uintmax_t size = 0;
    long modified_ms = 0;
  };

  struct DirectoryEntry
  {
    std::string name;
    std::string path;
    FileSystemEntryType type = FileSystemEntryType::OTHER;
    bool hidden = false;
  };

  class FileSystem
  {
   public:
    virtual ~FileSystem() = default;

    virtual const std::string read(const std::string& file_name) = 0;
    virtual void write(const std::string& file_name, const std::string& contents);
    virtual void copy_file(const std::string& source, const std::string& destination);
    virtual void remove_tree(const std::string& path);
    virtual void create_symlink(const std::string& source, const std::string& link);
    virtual bool is_symlink(const std::string& path);
    virtual std::string read_symlink(const std::string& path);
    virtual std::vector<DirectoryEntry> list_directory(const std::string& path);
    virtual FileSystemStat stat(const std::string& path);
    virtual bool exists(const std::string& path);
    virtual bool is_file(const std::string& path);
    virtual bool is_directory(const std::string& path);
    virtual std::string absolute_path(const std::string& path);
    virtual std::string current_directory();
    virtual std::string home_directory();
  };
} // namespace Roo

#endif

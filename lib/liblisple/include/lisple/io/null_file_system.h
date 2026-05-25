#ifndef LISPLE__IO__NULL_FILE_SYSTEM_H
#define LISPLE__IO__NULL_FILE_SYSTEM_H

#include <lisple/io/file_system.h>

namespace Lisple
{
  class NullFileSystem : public FileSystem
  {
   public:
    const std::string read(const std::string& file_name) override;
    void write(const std::string& file_name, const std::string& contents) override;
    void copy_file(const std::string& source, const std::string& destination) override;
    void remove_tree(const std::string& path) override;
    void create_symlink(const std::string& source, const std::string& link) override;
    bool is_symlink(const std::string& path) override;
    std::string read_symlink(const std::string& path) override;
    std::vector<DirectoryEntry> list_directory(const std::string& path) override;
    FileSystemStat stat(const std::string& path) override;
    bool exists(const std::string& path) override;
    bool is_file(const std::string& path) override;
    bool is_directory(const std::string& path) override;
    std::string absolute_path(const std::string& path) override;
    std::string current_directory() override;
    std::string home_directory() override;
  };
} // namespace Lisple

#endif

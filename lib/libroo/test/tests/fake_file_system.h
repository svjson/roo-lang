
#ifndef __FAKE_FILE_SYSTEM_H_
#define __FAKE_FILE_SYSTEM_H_

#include <map>
#include <string>
#include <vector>

#include <roo/io/file_system.h>

namespace RooTest
{
  class FakeFileSystem : public Roo::FileSystem
  {
    std::map<std::string, std::string> fs_contents;
    std::map<std::string, std::string> fs_symlinks;

   public:
    const std::string read(const std::string& file_name) override;

    void add_file(const std::string& file_name, const std::string& file_contents);
    bool has_file(const std::string& file_name) const;
    const std::string& get_file(const std::string& file_name) const;
    void write(const std::string& file_name, const std::string& contents) override;
    void copy_file(const std::string& source, const std::string& destination) override;
    void remove_tree(const std::string& path) override;
    void create_symlink(const std::string& source, const std::string& link) override;
    bool is_symlink(const std::string& path) override;
    std::string read_symlink(const std::string& path) override;
    std::vector<Roo::DirectoryEntry> list_directory(const std::string& path) override;
    Roo::FileSystemStat stat(const std::string& path) override;
    bool exists(const std::string& path) override;
    bool is_file(const std::string& path) override;
    bool is_directory(const std::string& path) override;
    std::string absolute_path(const std::string& path) override;
    std::string current_directory() override;
    std::string home_directory() override;
  };
} // namespace RooTest

#endif

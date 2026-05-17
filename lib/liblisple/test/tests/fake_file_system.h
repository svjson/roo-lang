
#ifndef __FAKE_FILE_SYSTEM_H_
#define __FAKE_FILE_SYSTEM_H_

#include <map>
#include <string>
#include <vector>

#include <lisple/io/file_system.h>

namespace LispleTest
{
  class FakeFileSystem : public Lisple::FileSystem
  {
    std::map<std::string, std::string> fs_contents;

   public:
    const std::string read(const std::string& file_name) override;

    void add_file(const std::string& file_name, const std::string& file_contents);
    bool has_file(const std::string& file_name) const;
    const std::string& get_file(const std::string& file_name) const;
    void write(const std::string& file_name, const std::string& contents) override;
    std::vector<Lisple::DirectoryEntry> list_directory(const std::string& path) override;
    Lisple::FileSystemStat stat(const std::string& path) override;
    bool exists(const std::string& path) override;
    bool is_file(const std::string& path) override;
    bool is_directory(const std::string& path) override;
    std::string absolute_path(const std::string& path) override;
    std::string current_directory() override;
    std::string home_directory() override;
  };
} // namespace LispleTest

#endif

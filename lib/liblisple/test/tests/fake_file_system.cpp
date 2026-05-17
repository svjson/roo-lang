
#include "fake_file_system.h"

namespace LispleTest
{
  const std::string FakeFileSystem::read(const std::string& file_name)
  {
    return fs_contents.at(file_name);
  }

  void FakeFileSystem::add_file(const std::string& file_name,
                                const std::string& file_contents)
  {
    fs_contents.emplace(file_name, file_contents);
  }

  bool FakeFileSystem::has_file(const std::string& file_name) const
  {
    return fs_contents.count(file_name) > 0;
  }

  const std::string& FakeFileSystem::get_file(const std::string& file_name) const
  {
    return fs_contents.at(file_name);
  }

  void FakeFileSystem::write(const std::string& file_name, const std::string& contents)
  {
    fs_contents[file_name] = contents;
  }
} // namespace LispleTest

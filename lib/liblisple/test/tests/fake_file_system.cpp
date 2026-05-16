
#include "fake_file_system.h"

namespace LispleTest
{
  const std::string FakeFileSystem::read_file_to_string(const std::string& file_name)
  {
    return fs_contents.at(file_name);
  }

  void FakeFileSystem::add_file(const std::string& file_name, const std::string& file_contents)
  {
    fs_contents.emplace(file_name, file_contents);
  }
}

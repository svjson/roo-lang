
#ifndef __FAKE_FILE_SYSTEM_H_
#define __FAKE_FILE_SYSTEM_H_

#include <map>
#include <string>

#include <lisple/file_system.h>

namespace LispleTest
{
  class FakeFileSystem : public Lisple::FileSystem
  {
    std::map<const std::string, const std::string> fs_contents;

   public:
    const std::string read_file_to_string(const std::string& file_name) override;

    void add_file(const std::string& file_name, const std::string& file_contents);
  };
}

#endif

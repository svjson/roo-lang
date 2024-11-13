
#include "dir_root_file_system.h"

#include <fstream>
#include <string>
#include <sstream>

#include "exception.h"

namespace Lisple
{
  DirRootFileSystem::DirRootFileSystem(const std::string& dir_root)
    : dir_root(dir_root)
  {

  }

  const std::string DirRootFileSystem::read_file_to_string(const std::string& file_name)
  {
    const std::string relative_path = dir_root + "/" + file_name;
    std::ifstream stream(relative_path);
    if (stream.fail())
    {
      throw LispleException("File not found: " + relative_path);
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    stream.close();
    return buffer.str();
  }

}

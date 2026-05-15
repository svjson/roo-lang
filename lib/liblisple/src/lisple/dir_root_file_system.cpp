
#include "lisple/dir_root_file_system.h"

#include <fstream>
#include <sstream>
#include <string>

#include <lisple/exception.h>

namespace Lisple
{
  namespace
  {
    std::string read_file(const std::string& path)
    {
      std::ifstream stream(path);
      if (stream.fail())
      {
        stream.close();
        throw LispleException("File not found on load path: '" + path + "'");
      }
      std::stringstream buffer;
      buffer << stream.rdbuf();
      stream.close();
      return buffer.str();
    }
  } // namespace

  DirRootFileSystem::DirRootFileSystem(const std::string& dir_root)
    : DirRootFileSystem(std::vector<std::string>{dir_root})
  {
  }

  DirRootFileSystem::DirRootFileSystem(const std::vector<std::string>& load_paths)
    : load_paths(load_paths)
  {
  }

  const std::string DirRootFileSystem::read_file_to_string(const std::string& file_name)
  {
    for (auto& load_path : load_paths)
    {
      const std::string relative_path = load_path + "/" + file_name;
      try
      {
        return read_file(relative_path);
      }
      catch (const LispleException&)
      {
      }
    }
    throw LispleException("File not found on load path: '" + file_name + "'");
  }

} // namespace Lisple

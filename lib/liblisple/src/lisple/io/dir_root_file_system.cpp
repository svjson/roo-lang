
#include "lisple/io/dir_root_file_system.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <lisple/exception.h>

namespace Lisple
{
  namespace
  {
    std::string resolve_path(const std::string& load_path, const std::string& file_name)
    {
      return load_path + "/" + file_name;
    }

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

  const std::string DirRootFileSystem::read(const std::string& file_name)
  {
    for (auto& load_path : load_paths)
    {
      const std::string relative_path = resolve_path(load_path, file_name);
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

  void DirRootFileSystem::write(const std::string& file_name, const std::string& contents)
  {
    const std::string root = load_paths.empty() ? "." : load_paths.front();
    const std::filesystem::path path = resolve_path(root, file_name);
    const auto parent_path = path.parent_path();
    if (!parent_path.empty())
    {
      std::filesystem::create_directories(parent_path);
    }

    std::ofstream stream(path);
    if (stream.fail())
    {
      stream.close();
      throw LispleException("Could not write file: '" + path.string() + "'");
    }
    stream << contents;
    stream.close();
  }

} // namespace Lisple

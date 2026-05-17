
#ifndef LISPLE__IO__DIR_ROOT_FILE_SYSTEM_H
#define LISPLE__IO__DIR_ROOT_FILE_SYSTEM_H

#include <string>
#include <vector>

#include <lisple/io/file_system.h>

namespace Lisple
{
  class DirRootFileSystem : public FileSystem
  {
   private:
    std::vector<std::string> load_paths;

   public:
    DirRootFileSystem(const std::string& root_path);
    DirRootFileSystem(const std::vector<std::string>& load_paths);

    const std::string read(const std::string& file_name) override;
    void write(const std::string& file_name, const std::string& contents) override;
  };
} // namespace Lisple

#endif

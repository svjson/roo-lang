
#ifndef __DIR_ROOT_FILE_SYSTEM_H_
#define __DIR_ROOT_FILE_SYSTEM_H_

#include "file_system.h"
#include <string>
#include <vector>

namespace Lisple
{
  class DirRootFileSystem : public FileSystem
  {
   private:
    std::vector<std::string> load_paths;

   public:
    DirRootFileSystem(const std::string& root_path);
    DirRootFileSystem(const std::vector<std::string>& load_paths);

    const std::string read_file_to_string(const std::string& file_name) override;
  };
} // namespace Lisple

#endif

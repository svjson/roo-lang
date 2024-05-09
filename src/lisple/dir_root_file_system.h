
#ifndef __DIR_ROOT_FILE_SYSTEM_H_
#define __DIR_ROOT_FILE_SYSTEM_H_

#include <string>

#include "file_system.h"

namespace Lisple
{
  class DirRootFileSystem : public FileSystem
  {
   private:
    const std::string dir_root;

   public:
    DirRootFileSystem(const std::string& root_path);

    const std::string read_file_to_string(const std::string& file_name) override;
  };
}

#endif

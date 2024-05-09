
#ifndef __FILE_SYSTEM_H_
#define __FILE_SYSTEM_H_

#include <string>

namespace Lisple
{
  class FileSystem
  {
   public:
    virtual ~FileSystem() = default;

    virtual const std::string read_file_to_string(const std::string& file_name) = 0;
  };
}

#endif

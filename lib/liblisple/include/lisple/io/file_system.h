
#ifndef LISPLE__IO__FILE_SYSTEM_H
#define LISPLE__IO__FILE_SYSTEM_H

#include <string>

namespace Lisple
{
  class FileSystem
  {
   public:
    virtual ~FileSystem() = default;

    virtual const std::string read(const std::string& file_name) = 0;
    virtual void write(const std::string& file_name, const std::string& contents);
  };
} // namespace Lisple

#endif

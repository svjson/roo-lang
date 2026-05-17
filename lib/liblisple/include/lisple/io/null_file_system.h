#ifndef LISPLE__IO__NULL_FILE_SYSTEM_H
#define LISPLE__IO__NULL_FILE_SYSTEM_H

#include <lisple/io/file_system.h>

namespace Lisple
{
  class NullFileSystem : public FileSystem
  {
   public:
    const std::string read(const std::string& file_name) override;
    void write(const std::string& file_name, const std::string& contents) override;
  };
} // namespace Lisple

#endif

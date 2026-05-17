#include "lisple/io/file_system.h"

#include <lisple/exception.h>

namespace Lisple
{
  void FileSystem::write(const std::string& file_name, const std::string&)
  {
    throw LispleException("File system does not support writing: '" + file_name + "'");
  }
} // namespace Lisple

#include "lisple/io/null_file_system.h"

#include <lisple/exception.h>

namespace Lisple
{
  const std::string NullFileSystem::read(const std::string& file_name)
  {
    throw LispleException("This Lisple context does not provide any file system access: '" +
                          file_name + "'");
  }

  void NullFileSystem::write(const std::string& file_name, const std::string&)
  {
    throw LispleException("This Lisple context does not provide any file system access: '" +
                          file_name + "'");
  }
} // namespace Lisple

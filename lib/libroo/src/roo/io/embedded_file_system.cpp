#include "roo/io/embedded_file_system.h"

#include <filesystem>
#include <string>
#include <string_view>

#include <roo/exception.h>

namespace Roo
{
  namespace
  {
    std::string normalize_path(std::string_view path)
    {
      return std::filesystem::path(path).lexically_normal().generic_string();
    }
  } // namespace

  EmbeddedFileSystem::EmbeddedFileSystem(std::span<const EmbeddedFile> files)
  {
    this->files.reserve(files.size());
    for (const auto& file : files)
    {
      this->files.insert_or_assign(normalize_path(file.path), file.contents);
    }
  }

  const std::string EmbeddedFileSystem::read(const std::string& file_name)
  {
    const auto file = files.find(normalize_path(file_name));
    if (file == files.end())
    {
      throw IOException("Embedded file not found: '" + file_name + "'");
    }
    return std::string(file->second);
  }
} // namespace Roo

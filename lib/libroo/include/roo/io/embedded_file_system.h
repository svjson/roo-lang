#ifndef ROO__IO__EMBEDDED_FILE_SYSTEM_H
#define ROO__IO__EMBEDDED_FILE_SYSTEM_H

#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

#include <roo/io/file_system.h>

namespace Roo
{
  struct EmbeddedFile
  {
    std::string_view path;
    std::string_view contents;
  };

  /**
   * Read-only filesystem backed by source data embedded in the host binary.
   * The supplied file contents must outlive the filesystem.
   */
  class EmbeddedFileSystem : public FileSystem
  {
    std::unordered_map<std::string, std::string_view> files;

   public:
    explicit EmbeddedFileSystem(std::span<const EmbeddedFile> files);

    const std::string read(const std::string& file_name) override;
  };
} // namespace Roo

#endif


#ifndef LISPLE__IO__FILE_SYSTEM_NAMESPACE_SOURCE_H
#define LISPLE__IO__FILE_SYSTEM_NAMESPACE_SOURCE_H

#include <optional>
#include <string>
#include <vector>

#include <lisple/namespace_source.h>

namespace Lisple
{
  class FileSystem;

  /**
   * Resolves namespace names to source files under a FileSystem.
   * Dots in namespace names map to path separators; dashes are preserved as-is.
   * Extensions are tried in priority order (default: .lisple, .lspl).
   */
  class FileSystemNamespaceSource : public NamespaceSource
  {
    FileSystem* fs;
    std::vector<std::string> extensions;

    std::string ns_to_path(const std::string& ns_name) const;

    /**
     * @brief Resolve the expected disk path of a namespace in relation to
     * the source of an importing namespace.
     *
     * Given that current_ns_name is known to have been read from
     * current_source_path, the number of namespace segments that map to
     * directories in the path can be derived. Any namespace sharing the
     * same root segment can then be resolved relative to the same root
     * directory.
     *
     * @return an empty string when inference is not applicable.
     */
    std::string infer_path(const std::string& ns_name,
                           const std::string& current_ns_name,
                           const std::string& current_source_path) const;

   public:
    explicit FileSystemNamespaceSource(FileSystem* fs);
    FileSystemNamespaceSource(FileSystem* fs, std::vector<std::string> extensions);

    std::optional<NamespaceFetchResult> fetch(
      const std::string& ns_name,
      const NamespaceResolutionContext& ctx) override;
  };
} // namespace Lisple

#endif

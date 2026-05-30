
#ifndef __ROO_NAMESPACE_LOADER_H_
#define __ROO_NAMESPACE_LOADER_H_

#include <string>
#include <vector>

namespace Roo
{
  class NamespaceSource;
  class Runtime;

  class NamespaceLoader
  {
    struct LoadingEntry
    {
      std::string ns_name; // empty for read_file sentinels
      std::string resolved_path;
    };

    std::vector<NamespaceSource*> sources;
    std::vector<LoadingEntry> loading_stack;

   public:
    explicit NamespaceLoader(NamespaceSource* source);
    explicit NamespaceLoader(std::vector<NamespaceSource*> sources);

    /**
     * @brief Ensure the named namespace is loaded into the runtime.
     *
     * @returns true if the namespace was found (already loaded or
     * successfully loaded now), false if no source could provide it.
     *
     * @throws CyclicNamespaceException if a circular dependency is detected.
     */
    bool load(Runtime& runtime, const std::string& ns_name);

    void apply_metadata(Runtime& runtime, const std::string& ns_name);

    void push_file_context(const std::string& resolved_path);
    void pop_file_context();

    bool is_loading(const std::string& ns_name) const;
  };
} // namespace Roo

#endif

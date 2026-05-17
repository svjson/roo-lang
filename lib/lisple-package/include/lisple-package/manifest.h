#ifndef LISPLE_PACKAGE_MANIFEST_H
#define LISPLE_PACKAGE_MANIFEST_H

#include <memory>
#include <string>
#include <vector>

#include <lisple/io/file_system.h>

namespace Lisple::Package
{
  struct Dependency
  {
    std::string name;
    std::string version;
    std::string path;
  };

  struct NativeLibrary
  {
    std::string name;
    std::string version;
    std::string path;
    std::string package_root;
    std::vector<std::string> namespaces;
  };

  struct Manifest
  {
    std::string name;
    std::string version;
    std::string description;
    std::vector<Dependency> dependencies;
    std::vector<std::string> load_roots;
    std::vector<std::string> native_namespaces;
    std::vector<NativeLibrary> native_libraries;
    std::vector<std::string> entry_points;
    std::vector<std::string> test_entry_points;
  };

  struct LoadPlan
  {
    std::string package_root;
    std::vector<std::string> package_roots;
    std::vector<std::string> load_paths;
    std::vector<std::string> native_namespaces;
    std::vector<NativeLibrary> native_libraries;
    std::vector<std::string> entry_points;
    std::vector<std::string> test_entry_points;
  };

  struct ResolveOptions
  {
    std::vector<std::string> package_search_roots;
  };

  Manifest parse_manifest(const std::string& source,
                          const std::string& source_name = "package.edn");
  Manifest read_manifest(Lisple::FileSystem& fs,
                         const std::string& manifest_path = "package.edn");
  LoadPlan build_load_plan(const Manifest& manifest, const std::string& package_root);
  LoadPlan resolve_load_plan(Lisple::FileSystem& fs,
                             const std::string& package_root,
                             const ResolveOptions& options = {});

  // Return runtime load paths with host-provided paths first and resolved package
  // source roots appended in dependency-first order.
  std::vector<std::string> merge_load_paths(
    const LoadPlan& plan,
    const std::vector<std::string>& extra_load_paths = {});

  // Convenience bridge for embedders that want the standard directory-root
  // filesystem. Callers must keep the returned filesystem alive for the runtime.
  std::unique_ptr<Lisple::FileSystem> make_load_path_file_system(
    const LoadPlan& plan,
    const std::vector<std::string>& extra_load_paths = {});
} // namespace Lisple::Package

#endif

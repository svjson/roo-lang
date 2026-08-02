#ifndef ROO_PACKAGE_MANIFEST_H
#define ROO_PACKAGE_MANIFEST_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <roo/io/file_system.h>
#include <roo/namespace_source.h>

namespace Roo
{
  class Runtime;
}

namespace Roo::Package
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
    std::map<std::string, std::string> runtimes;
    std::vector<Roo::NamespaceRoot> namespace_roots;
    std::vector<std::string> native_namespaces;
    std::vector<NativeLibrary> native_libraries;
    std::vector<std::string> autoloads;
    std::map<std::string, std::string> config;
    std::map<std::string, std::string> tools;
    std::vector<std::string> entry_points;
    std::string main;
    std::string run;
  };

  struct PackageInfo
  {
    std::string name;
    std::string version;
    std::string package_root;
    std::vector<std::string> load_roots;
    std::map<std::string, std::string> runtimes;
    std::map<std::string, std::string> config;
    std::map<std::string, std::string> tools;
  };

  struct LoadPlan
  {
    std::string package_root;
    std::vector<std::string> package_roots;
    std::vector<PackageInfo> packages;
    std::vector<std::string> load_paths;
    std::map<std::string, std::string> runtimes;
    std::vector<Roo::NamespaceRoot> namespace_roots;
    std::vector<std::string> native_namespaces;
    std::vector<NativeLibrary> native_libraries;
    std::vector<std::string> autoloads;
    std::vector<std::string> entry_points;
    std::string main;
    std::string run;
  };

  struct ResolveOptions
  {
    std::vector<std::string> package_search_roots;
  };

  std::string default_local_repository_root();
  std::vector<std::string> default_package_search_roots();

  Manifest parse_manifest(const std::string& source,
                          const std::string& source_name = "package.edn");
  Manifest read_manifest(Roo::FileSystem& fs,
                         const std::string& manifest_path = "package.edn");
  LoadPlan build_load_plan(const Manifest& manifest, const std::string& package_root);
  LoadPlan resolve_load_plan(Roo::FileSystem& fs,
                             const std::string& package_root,
                             const ResolveOptions& options = {});

  /**
   * Return runtime load paths with host-provided paths first and
   * resolved package source roots appended in dependency-first order.
   */
  std::vector<std::string> merge_load_paths(
    const LoadPlan& plan,
    const std::vector<std::string>& extra_load_paths = {});

  /**
   * Convenience bridge for embedders that want the standard
   * directory-root filesystem. Callers must keep the returned
   * filesystem alive for the runtime.
   */
  std::unique_ptr<Roo::FileSystem> make_load_path_file_system(
    const LoadPlan& plan,
    const std::vector<std::string>& extra_load_paths = {});

  /**
   * Configure a runtime to use package-declared namespace roots. Call
   * this after constructing the runtime with a filesystem that can
   * read the resolved package paths.
   */
  void configure_runtime_namespace_roots(Roo::Runtime& runtime, const LoadPlan& plan);

  /**
   * Load package-declared bootstrap namespaces. Call this after
   * native package * libraries are loaded, so autoload namespaces may
   * require native namespaces.
   */
  void load_autoloads(Roo::Runtime& runtime, const LoadPlan& plan);
} // namespace Roo::Package

#endif

#ifndef ROO_PACKAGE_RUNTIME_ENVIRONMENT_H
#define ROO_PACKAGE_RUNTIME_ENVIRONMENT_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <roo/io/file_system.h>
#include <roo/namespace_source.h>
#include <roo/runtime/worker.h>
#include <roo/source.h>

#include <roo-package/manifest.h>

namespace Roo::Package
{
  /**
   * @brief Owned host inputs used to construct one application Runtime.
   *
   * Supporting file systems remain alive after the namespace source is moved
   * into the Runtime. This supports hosts whose namespace source uses a
   * different backing store from application-visible file IO.
   */
  struct RuntimeEnvironmentInputs
  {
    std::unique_ptr<FileSystem> file_system;
    std::unique_ptr<NamespaceSource> namespace_source;
    std::vector<std::unique_ptr<FileSystem>> supporting_file_systems;
  };

  using RuntimeEnvironmentInputsFactory = std::function<RuntimeEnvironmentInputs()>;

  /**
   * @brief Replayable specification for a package-configured Runtime.
   *
   * The specification reconstructs filesystem and namespace-source ownership,
   * applies the resolved package plan, loads fresh native package namespaces,
   * and evaluates autoloads. It does not invoke a package tool, main function,
   * or entry point.
   */
  struct ApplicationRuntimeSpec
  {
    LoadPlan load_plan;
    RuntimeEnvironmentInputsFactory make_inputs;
    RuntimeOptions runtime_options;
    std::string autoload_loader_namespace = "roo.package.autoload";
    std::string autoload_source_name = "<package-autoload>";
  };

  /**
   * @brief Own one package-configured application Runtime and its host inputs.
   *
   * Runtime-bound values must be destroyed before this owner. Current-ABI native
   * libraries remain mapped for process lifetime, including after this owner is
   * destroyed.
   */
  class ApplicationRuntime
  {
   public:
    explicit ApplicationRuntime(const ApplicationRuntimeSpec& spec);
    ~ApplicationRuntime();

    ApplicationRuntime(const ApplicationRuntime&) = delete;
    ApplicationRuntime& operator=(const ApplicationRuntime&) = delete;
    ApplicationRuntime(ApplicationRuntime&&) = delete;
    ApplicationRuntime& operator=(ApplicationRuntime&&) = delete;

    /** @brief Return the owned application Runtime. */
    Runtime& runtime();

   private:
    struct Impl;
    std::unique_ptr<Impl> impl;
  };

  /**
   * @brief Create a reusable factory from an application runtime specification.
   * @param spec Replayable package runtime specification.
   * @return Factory suitable for parent construction and worker registration.
   */
  WorkerEnvironmentFactory make_application_runtime_factory(ApplicationRuntimeSpec spec);

  /**
   * @brief Build the standard directory-backed application runtime specification.
   * @param plan Resolved package load plan.
   * @param extra_load_paths Host load paths placed before package paths.
   * @param options Runtime diagnostic and evaluation options.
   * @return Replayable directory-backed application runtime specification.
   */
  ApplicationRuntimeSpec make_directory_application_runtime_spec(
    LoadPlan plan,
    std::vector<std::string> extra_load_paths = {},
    RuntimeOptions options = {});
} // namespace Roo::Package

#endif

#include <utility>

#include <roo/exception.h>
#include <roo/io/dir_root_file_system.h>
#include <roo/io/file_system_namespace_source.h>
#include <roo/runtime.h>

#include <roo-package/native_loader.h>
#include <roo-package/runtime_environment.h>

namespace Roo::Package
{
  namespace
  {
    RuntimeEnvironmentInputs make_inputs(const ApplicationRuntimeSpec& spec)
    {
      if (!spec.make_inputs)
      {
        throw RooException("Application runtime input factory must not be empty.");
      }
      return spec.make_inputs();
    }

    class ApplicationWorkerEnvironment final : public WorkerEnvironment
    {
     private:
      ApplicationRuntime application;

     public:
      explicit ApplicationWorkerEnvironment(const ApplicationRuntimeSpec& spec)
        : application(spec)
      {
      }

      Runtime& runtime() override { return application.runtime(); }
    };
  } // namespace

  struct ApplicationRuntime::Impl
  {
    RuntimeEnvironmentInputs inputs;
    LoadedNativePackages native_packages;
    Runtime instance;

    explicit Impl(const ApplicationRuntimeSpec& spec)
      : inputs(make_inputs(spec))
      , instance(inputs.file_system.get(), std::move(inputs.namespace_source))
    {
      instance.get_options() = spec.runtime_options;
      configure_runtime_namespace_roots(instance, spec.load_plan);
      native_packages = load_native_libraries(instance, spec.load_plan);
      load_autoloads(instance,
                     spec.load_plan,
                     spec.autoload_loader_namespace,
                     spec.autoload_source_name);
    }
  };

  ApplicationRuntime::ApplicationRuntime(const ApplicationRuntimeSpec& spec)
    : impl(std::make_unique<Impl>(spec))
  {
  }

  ApplicationRuntime::~ApplicationRuntime() = default;

  Runtime& ApplicationRuntime::runtime()
  {
    return impl->instance;
  }

  WorkerEnvironmentFactory make_application_runtime_factory(ApplicationRuntimeSpec spec)
  {
    return [spec = std::move(spec)]()
    {
      return std::make_unique<ApplicationWorkerEnvironment>(spec);
    };
  }

  ApplicationRuntimeSpec make_directory_application_runtime_spec(
    LoadPlan plan,
    std::vector<std::string> extra_load_paths,
    RuntimeOptions options)
  {
    std::vector<std::string> load_paths = merge_load_paths(plan, extra_load_paths);
    RuntimeEnvironmentInputsFactory make_directory_inputs =
      [load_paths = std::move(load_paths)]()
    {
      RuntimeEnvironmentInputs inputs;
      inputs.file_system = std::make_unique<DirRootFileSystem>(load_paths);
      inputs.namespace_source =
        std::make_unique<FileSystemNamespaceSource>(inputs.file_system.get());
      return inputs;
    };

    return {std::move(plan), std::move(make_directory_inputs), options};
  }
} // namespace Roo::Package

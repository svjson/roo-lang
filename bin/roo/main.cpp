#include <algorithm>
#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <roo/exception.h>
#include <roo/io/dir_root_file_system.h>
#include <roo/runtime.h>

#include <roo-package/application.h>
#include <roo-package/manifest.h>
#include <roo-package/runtime_environment.h>

namespace
{
  void print_usage()
  {
    std::cout << "Usage: roo [--help|--version] [--load-path <path>] "
                 "[-R|--package-repository <dir>]... <file|package-tool> [args...]\n";
  }

  void print_help()
  {
    std::cout << "roo: run a Roo file with the Roo runtime\n"
                 "Usage:\n"
                 "  roo <file>\n"
                 "  roo --help\n"
                 "  roo --version\n"
                 "  roo --load-path <path> <file>\n"
                 "  roo --load-path <path1> --load-path <path2> <file>\n"
                 "  roo [-R|--package-repository <dir>]... <dependency-name>\n"
                 "\n"
                 "Options:\n"
                 "  -R, --package-repository <dir>\n"
                 "      Search this package repository before sibling packages and the "
                 "user-local repository. Repeatable.\n"
                 "\n"
                 "Arguments after <file|package-tool> are forwarded to that target.\n";
  }

  void print_version()
  {
    std::cout << "roo " << ROO_VERSION << "\n";
  }

  void print_error_and_usage(const std::string& message)
  {
    std::cerr << message << "\n";
    print_usage();
  }

  bool is_directory_target(const std::string& path)
  {
    std::error_code ec;
    const bool is_directory = std::filesystem::is_directory(path, ec);
    return !ec && is_directory;
  }

  bool path_exists(const std::string& path)
  {
    std::error_code ec;
    const bool exists = std::filesystem::exists(path, ec);
    return !ec && exists;
  }

  bool is_bare_tool_target(const std::string& path)
  {
    return !path.empty() && path != "." && path != ".." &&
           path.find('/') == std::string::npos && path.find('\\') == std::string::npos;
  }

  void run_package_entry_points(Roo::Runtime& runtime,
                                const Roo::Package::LoadPlan& package_plan,
                                const std::string& target_path)
  {
    if (package_plan.entry_points.empty())
    {
      throw Roo::RooException(
        "Directory target '" + target_path +
        "' resolves to a package with no :entry-points in package.edn.");
    }

    for (const auto& entry_point : package_plan.entry_points)
    {
      runtime.eval("(ns roo.cli.entry (:require " + entry_point + "))", "<package-entry>");
    }
  }

  int run_package_main(Roo::Runtime& runtime,
                       const Roo::Package::LoadPlan& package_plan,
                       const std::vector<std::string>& args)
  {
    return Roo::Package::Application::exit_code(
      Roo::Package::Application::invoke_main(runtime, package_plan.main, args));
  }

  int run_target(Roo::Runtime& runtime,
                 const std::optional<Roo::Package::LoadPlan>& package_plan,
                 const std::string& file_path,
                 const std::vector<std::string>& app_args,
                 bool run_package,
                 bool run_tool)
  {
    if (run_tool && package_plan)
    {
      return Roo::Package::Application::exit_code(
        Roo::Package::Application::invoke_tool(runtime,
                                               *package_plan,
                                               file_path,
                                               "run",
                                               app_args));
    }
    if (run_package && package_plan && !package_plan->main.empty())
    {
      return run_package_main(runtime, *package_plan, app_args);
    }
    if (run_package)
    {
      run_package_entry_points(runtime, *package_plan, file_path);
    }
    else
    {
      runtime.read_file(file_path);
    }
    return 0;
  }

  int run_target_reporting_errors(Roo::Runtime& runtime,
                                  const std::optional<Roo::Package::LoadPlan>& package_plan,
                                  const std::string& file_path,
                                  const std::vector<std::string>& app_args,
                                  bool run_package,
                                  bool run_tool)
  {
    try
    {
      return run_target(runtime, package_plan, file_path, app_args, run_package, run_tool);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }

} // namespace

int main(int argc, char** argv)
{
  std::string file_path;
  std::vector<std::string> app_args;
  std::vector<std::string> load_paths{std::filesystem::current_path().string(), "/"};
  std::vector<std::string> package_repository_roots;

  for (int i = 1; i < argc; ++i)
  {
    const std::string arg = argv[i];

    if (!file_path.empty())
    {
      app_args.push_back(arg);
      continue;
    }

    if (arg == "-h" || arg == "--help")
    {
      print_help();
      return 0;
    }

    if (arg == "--version")
    {
      print_version();
      return 0;
    }

    if (arg == "--load-path")
    {
      if (i + 1 >= argc)
      {
        print_error_and_usage("Missing value for --load-path.");
        return 1;
      }

      const std::string load_path = argv[++i];
      if (load_path.empty() || (!load_path.empty() && load_path[0] == '-'))
      {
        print_error_and_usage("Invalid value for --load-path: " + load_path);
        return 1;
      }
      load_paths.push_back(load_path);
      continue;
    }

    if (arg.rfind("--load-path=", 0) == 0)
    {
      const std::string load_path = arg.substr(std::string("--load-path=").size());
      if (load_path.empty())
      {
        print_error_and_usage("Invalid value for --load-path.");
        return 1;
      }
      load_paths.push_back(load_path);
      continue;
    }

    if (arg == "-R" || arg == "--package-repository")
    {
      if (i + 1 >= argc)
      {
        print_error_and_usage("Missing value for --package-repository.");
        return 1;
      }

      const std::string package_repository = argv[++i];
      if (package_repository.empty() || package_repository[0] == '-')
      {
        print_error_and_usage("Invalid value for --package-repository: " +
                              package_repository);
        return 1;
      }
      package_repository_roots.push_back(package_repository);
      continue;
    }

    if (arg.rfind("--package-repository=", 0) == 0)
    {
      const std::string package_repository =
        arg.substr(std::string("--package-repository=").size());
      if (package_repository.empty())
      {
        print_error_and_usage("Invalid value for --package-repository.");
        return 1;
      }
      package_repository_roots.push_back(package_repository);
      continue;
    }

    if (arg.rfind("-", 0) == 0)
    {
      print_error_and_usage("Unknown option: " + arg);
      return 1;
    }

    file_path = arg;
  }

  if (file_path.empty())
  {
    print_error_and_usage("No file provided.");
    return 1;
  }

  try
  {
    Roo::DirRootFileSystem manifest_fs("/");
    const bool run_package = is_directory_target(file_path);
    const bool run_tool =
      !run_package && !path_exists(file_path) && is_bare_tool_target(file_path);
    std::optional<Roo::Package::LoadPlan> package_plan;
    const auto package_root = Roo::Package::find_package_root(manifest_fs, file_path);
    if (package_root)
    {
      Roo::Package::ResolveOptions resolve_options{package_repository_roots};
      if (run_tool)
      {
        const Roo::Package::Manifest manifest = Roo::Package::read_manifest(
          manifest_fs,
          (std::filesystem::path(*package_root) / "package.edn").string());
        if (std::any_of(manifest.development.dependencies.begin(),
                        manifest.development.dependencies.end(),
                        [&](const Roo::Package::Dependency& dependency)
                        { return dependency.name == file_path; }))
        {
          resolve_options.root_scope = Roo::Package::ManifestScope::Development;
        }
      }
      package_plan =
        Roo::Package::resolve_load_plan(manifest_fs, *package_root, resolve_options);
    }
    else if (run_package)
    {
      throw Roo::RooException("Directory target '" + file_path +
                              "' is not inside a package.");
    }

    if (package_plan)
    {
      Roo::Package::ApplicationRuntimeSpec runtime_spec =
        Roo::Package::make_directory_application_runtime_spec(*package_plan, load_paths);
      Roo::Package::ApplicationRuntime application(runtime_spec);
      Roo::WorkerEnvironmentFactory environment_factory =
        Roo::Package::make_application_runtime_factory(std::move(runtime_spec));
      Roo::Runtime& runtime = application.runtime();
      runtime.worker_registry().register_environment("application", environment_factory);
      return run_target_reporting_errors(runtime,
                                         package_plan,
                                         file_path,
                                         app_args,
                                         run_package,
                                         run_tool);
    }

    Roo::DirRootFileSystem roo_fs(load_paths);
    Roo::Runtime runtime(&roo_fs);
    runtime.set_call_stack_diagnostics(true);
    return run_target_reporting_errors(runtime,
                                       package_plan,
                                       file_path,
                                       app_args,
                                       run_package,
                                       run_tool);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

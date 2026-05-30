#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <roo-package/application.h>
#include <roo-package/manifest.h>
#include <roo-package/native_loader.h>
#include <roo/exception.h>
#include <roo/io/dir_root_file_system.h>
#include <roo/runtime.h>

namespace
{
  void print_usage()
  {
    std::cout << "Usage: roo [--help|--version] [--load-path <path>] <file|package-tool> "
                 "[args...]\n";
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
                 "  roo <dependency-name>\n"
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

  std::optional<std::string> find_package_root(const std::filesystem::path& file_path)
  {
    std::error_code ec;
    auto current = std::filesystem::absolute(file_path, ec).lexically_normal();
    if (ec)
    {
      return std::nullopt;
    }

    if (std::filesystem::is_regular_file(current, ec))
    {
      current = current.parent_path();
    }
    if (ec)
    {
      ec.clear();
    }

    while (!current.empty())
    {
      const auto manifest_path = current / "package.edn";
      if (std::filesystem::is_regular_file(manifest_path, ec))
      {
        return current.string();
      }
      if (ec)
      {
        ec.clear();
      }

      const auto parent = current.parent_path();
      if (parent == current)
      {
        break;
      }
      current = parent;
    }

    return std::nullopt;
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

  void run_package_main(Roo::Runtime& runtime,
                        const Roo::Package::LoadPlan& package_plan,
                        const std::vector<std::string>& args)
  {
    Roo::Package::Application::invoke_main(runtime, package_plan.main, args);
  }

} // namespace

int main(int argc, char** argv)
{
  std::string file_path;
  std::vector<std::string> app_args;
  std::vector<std::string> load_paths{std::filesystem::current_path().string(), "/"};

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
    const auto package_root = find_package_root(file_path);
    if (package_root)
    {
      package_plan = Roo::Package::resolve_load_plan(manifest_fs, *package_root);
      load_paths = Roo::Package::merge_load_paths(*package_plan, load_paths);
    }
    else if (run_package)
    {
      throw Roo::RooException("Directory target '" + file_path +
                              "' is not inside a package.");
    }

    Roo::DirRootFileSystem roo_fs(load_paths);
    Roo::Package::LoadedNativePackages native_packages;
    Roo::Runtime runtime(&roo_fs);
    if (package_plan)
    {
      Roo::Package::configure_runtime_namespace_roots(runtime, *package_plan);
      native_packages = Roo::Package::load_native_libraries(runtime, *package_plan);
      Roo::Package::load_autoloads(runtime, *package_plan);
    }
    runtime.set_call_stack_diagnostics(true);
    if (run_tool && package_plan)
    {
      Roo::Package::Application::invoke_tool(runtime, *package_plan, file_path);
    }
    else if (run_package && package_plan && !package_plan->main.empty())
    {
      run_package_main(runtime, *package_plan, app_args);
    }
    else if (run_package)
    {
      run_package_entry_points(runtime, *package_plan, file_path);
    }
    else
    {
      runtime.read_file(file_path);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

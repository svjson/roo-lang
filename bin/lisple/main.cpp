#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <lisple/exception.h>
#include <lisple/io/dir_root_file_system.h>
#include <lisple/runtime.h>

#include <lisple-package/application.h>
#include <lisple-package/manifest.h>
#include <lisple-package/native_loader.h>

namespace
{
  void print_usage()
  {
    std::cout
      << "Usage: lisple [--help|--version] [--load-path <path>] <file|package-tool>\n";
  }

  void print_help()
  {
    std::cout << "lisple: run a lisple file with the Lisple runtime\n"
                 "Usage:\n"
                 "  lisple <file>\n"
                 "  lisple --help\n"
                 "  lisple --version\n"
                 "  lisple --load-path <path> <file>\n"
                 "  lisple --load-path <path1> --load-path <path2> <file>\n"
                 "  lisple <dependency-name>\n";
  }

  void print_version()
  {
    std::cout << "lisple " << LISPLE_VERSION << "\n";
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

  void run_package_entry_points(Lisple::Runtime& runtime,
                                const Lisple::Package::LoadPlan& package_plan,
                                const std::string& target_path)
  {
    if (package_plan.entry_points.empty())
    {
      throw Lisple::LispleException(
        "Directory target '" + target_path +
        "' resolves to a package with no :entry-points in package.edn.");
    }

    for (const auto& entry_point : package_plan.entry_points)
    {
      runtime.eval("(ns lisple.cli.entry (:require " + entry_point + "))",
                   "<package-entry>");
    }
  }

  void run_package_main(Lisple::Runtime& runtime,
                        const Lisple::Package::LoadPlan& package_plan,
                        const std::vector<std::string>& args)
  {
    Lisple::Package::Application::invoke_main(runtime, package_plan.main, args);
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

    if (!file_path.empty())
    {
      app_args.push_back(arg);
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
    Lisple::DirRootFileSystem manifest_fs("/");
    const bool run_package = is_directory_target(file_path);
    const bool run_tool =
      !run_package && !path_exists(file_path) && is_bare_tool_target(file_path);
    std::optional<Lisple::Package::LoadPlan> package_plan;
    const auto package_root = find_package_root(file_path);
    if (package_root)
    {
      package_plan = Lisple::Package::resolve_load_plan(manifest_fs, *package_root);
      load_paths = Lisple::Package::merge_load_paths(*package_plan, load_paths);
    }
    else if (run_package)
    {
      throw Lisple::LispleException("Directory target '" + file_path +
                                    "' is not inside a package.");
    }

    Lisple::DirRootFileSystem lisple_fs(load_paths);
    Lisple::Package::LoadedNativePackages native_packages;
    Lisple::Runtime runtime(&lisple_fs);
    if (package_plan)
    {
      Lisple::Package::configure_runtime_namespace_roots(runtime, *package_plan);
      native_packages = Lisple::Package::load_native_libraries(runtime, *package_plan);
      Lisple::Package::load_autoloads(runtime, *package_plan);
    }
    runtime.set_call_stack_diagnostics(true);
    if (run_tool && package_plan)
    {
      Lisple::Package::Application::invoke_tool(runtime, *package_plan, file_path);
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

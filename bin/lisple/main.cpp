#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <lisple/io/dir_root_file_system.h>
#include <lisple/runtime.h>
#include <lisple-package/manifest.h>

namespace
{
  void print_usage()
  {
    std::cout << "Usage: lisple [--help|--version] [--load-path <path>] <file>\n";
  }

  void print_help()
  {
    std::cout << "lisple: run a lisple file with the Lisple runtime\n"
                 "Usage:\n"
                 "  lisple <file>\n"
                 "  lisple --help\n"
                 "  lisple --version\n"
                 "  lisple --load-path <path> <file>\n"
                 "  lisple --load-path <path1> --load-path <path2> <file>\n";
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
} // namespace

int main(int argc, char** argv)
{
  std::string file_path;
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

    if (arg.rfind("-", 0) == 0)
    {
      print_error_and_usage("Unknown option: " + arg);
      return 1;
    }

    if (!file_path.empty())
    {
      print_error_and_usage("Expected only one file argument.");
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
    if (const auto package_root = find_package_root(file_path))
    {
      const auto package_plan =
        Lisple::Package::resolve_load_plan(manifest_fs, *package_root);
      load_paths = Lisple::Package::merge_load_paths(package_plan, load_paths);
    }

    Lisple::DirRootFileSystem lisple_fs(load_paths);
    Lisple::Runtime runtime(&lisple_fs);
    runtime.set_call_stack_diagnostics(true);
    runtime.read_file(file_path);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

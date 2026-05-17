#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <lisple/io/dir_root_file_system.h>
#include <lisple/runtime.h>

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
} // namespace

int main(int argc, char** argv)
{
  std::string file_path;
  std::vector<std::string> load_paths{"/"};

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

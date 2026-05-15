#include <exception>
#include <iostream>
#include <string>

#include <lisple/dir_root_file_system.h>
#include <lisple/runtime.h>

namespace
{
  void print_usage()
  {
    std::cout << "Usage: lisple [--help|--version] <file>\n";
  }

  void print_help()
  {
    std::cout << "lisple: run a lisple file with the Lisple runtime\n"
                 "Usage:\n"
                 "  lisple <file>\n"
                 "  lisple --help\n"
                 "  lisple --version\n";
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

    if (!arg.empty() && arg[0] == '-')
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
    Lisple::DirRootFileSystem lisple_fs("/");
    Lisple::Runtime runtime(&lisple_fs);
    runtime.read_file(file_path);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

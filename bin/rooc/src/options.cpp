#include <cstdlib>
#include <iostream>

#include <roo/exception.h>

#include <rooc/options.h>

namespace Rooc
{
  void print_usage()
  {
    std::cout << "Usage: rooc <generate|build> <package-dir> "
                 "[--build-dir <dir>] [--name <name>]\n";
  }

  void print_help()
  {
    std::cout << "rooc: generate and build a native Roo executable project\n"
                 "Usage:\n"
                 "  rooc generate <package-dir> [--build-dir <dir>] [--name <name>]\n"
                 "  rooc build <package-dir> [--build-dir <dir>] [--name <name>]\n"
                 "  rooc --help\n"
                 "  rooc --version\n";
  }

  void print_version()
  {
    std::cout << "rooc " << ROO_VERSION << "\n";
  }

  Options parse_args(int argc, char** argv)
  {
    Options options;

    if (argc <= 1)
    {
      throw Roo::RooException("No command provided.");
    }

    for (int i = 1; i < argc; ++i)
    {
      const std::string arg = argv[i];
      if (arg == "-h" || arg == "--help")
      {
        print_help();
        std::exit(0);
      }
      if (arg == "--version")
      {
        print_version();
        std::exit(0);
      }

      if (options.command.empty())
      {
        options.command = arg;
        continue;
      }

      if (arg == "--build-dir")
      {
        if (i + 1 >= argc)
        {
          throw Roo::RooException("Missing value for --build-dir.");
        }
        options.build_dir = argv[++i];
        continue;
      }
      if (arg.rfind("--build-dir=", 0) == 0)
      {
        options.build_dir = arg.substr(std::string("--build-dir=").size());
        continue;
      }

      if (arg == "--name")
      {
        if (i + 1 >= argc)
        {
          throw Roo::RooException("Missing value for --name.");
        }
        options.executable_name = argv[++i];
        continue;
      }
      if (arg.rfind("--name=", 0) == 0)
      {
        options.executable_name = arg.substr(std::string("--name=").size());
        continue;
      }

      if (!arg.empty() && arg.front() == '-')
      {
        throw Roo::RooException("Unknown option: " + arg);
      }

      if (!options.package_dir.empty())
      {
        throw Roo::RooException("Expected only one package directory.");
      }
      options.package_dir = arg;
    }

    if (options.command != "generate" && options.command != "build")
    {
      throw Roo::RooException("Unknown command: " + options.command);
    }
    if (options.package_dir.empty())
    {
      throw Roo::RooException("No package directory provided.");
    }
    if (options.build_dir.empty())
    {
      options.build_dir = ".rooc";
    }

    return options;
  }
} // namespace Rooc

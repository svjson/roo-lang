#include <cstdlib>
#include <iostream>

#include <lisple/exception.h>

#include <lisplec/options.h>

namespace Lisplec
{
  void print_usage()
  {
    std::cout << "Usage: lisplec <generate|build> <package-dir> "
                 "[--build-dir <dir>] [--name <name>]\n";
  }

  void print_help()
  {
    std::cout << "lisplec: generate and build a native Lisple executable project\n"
                 "Usage:\n"
                 "  lisplec generate <package-dir> [--build-dir <dir>] [--name <name>]\n"
                 "  lisplec build <package-dir> [--build-dir <dir>] [--name <name>]\n"
                 "  lisplec --help\n"
                 "  lisplec --version\n";
  }

  void print_version()
  {
    std::cout << "lisplec " << LISPLE_VERSION << "\n";
  }

  Options parse_args(int argc, char** argv)
  {
    Options options;

    if (argc <= 1)
    {
      throw Lisple::LispleException("No command provided.");
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
          throw Lisple::LispleException("Missing value for --build-dir.");
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
          throw Lisple::LispleException("Missing value for --name.");
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
        throw Lisple::LispleException("Unknown option: " + arg);
      }

      if (!options.package_dir.empty())
      {
        throw Lisple::LispleException("Expected only one package directory.");
      }
      options.package_dir = arg;
    }

    if (options.command != "generate" && options.command != "build")
    {
      throw Lisple::LispleException("Unknown command: " + options.command);
    }
    if (options.package_dir.empty())
    {
      throw Lisple::LispleException("No package directory provided.");
    }
    if (options.build_dir.empty())
    {
      options.build_dir = ".lisplec";
    }

    return options;
  }
} // namespace Lisplec

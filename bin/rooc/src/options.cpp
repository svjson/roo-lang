#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include <roo/exception.h>

#include <rooc/options.h>

namespace Rooc
{
  namespace
  {
    char path_separator()
    {
#if defined(_WIN32)
      return ';';
#else
      return ':';
#endif
    }

    std::filesystem::path absolute_normal_path(const std::filesystem::path& path)
    {
      std::error_code ec;
      auto absolute = std::filesystem::absolute(path, ec);
      if (ec)
      {
        return path.lexically_normal();
      }
      return absolute.lexically_normal();
    }

    std::filesystem::path resolve_executable_path(const char* argv0)
    {
      if (argv0 == nullptr || std::string(argv0).empty())
      {
        return {};
      }

      const std::filesystem::path raw_path(argv0);
      if (raw_path.has_parent_path())
      {
        return absolute_normal_path(raw_path);
      }

      const char* path_env = std::getenv("PATH");
      if (path_env != nullptr)
      {
        const std::string paths(path_env);
        size_t start = 0;
        while (start <= paths.size())
        {
          const size_t end = paths.find(path_separator(), start);
          const std::string entry =
            paths.substr(start, end == std::string::npos ? std::string::npos : end - start);
          if (!entry.empty())
          {
            const auto candidate = std::filesystem::path(entry) / raw_path;
            std::error_code ec;
            if (std::filesystem::exists(candidate, ec) && !ec)
            {
              return absolute_normal_path(candidate);
            }
          }
          if (end == std::string::npos)
          {
            break;
          }
          start = end + 1;
        }
      }

      return absolute_normal_path(raw_path);
    }
  } // namespace

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
    options.executable_path = resolve_executable_path(argc > 0 ? argv[0] : nullptr);

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

#include <cstdlib>
#include <filesystem>
#include <string>

#include <roo/exception.h>

#include <rooc/builder.h>

namespace Rooc
{
  namespace
  {
    std::string cmake_path(const std::filesystem::path& path)
    {
      return path.lexically_normal().generic_string();
    }

    std::string shell_arg(const std::string& value)
    {
      std::string result = "\"";
      for (const char c : value)
      {
        if (c == '"' || c == '\\' || c == '$' || c == '`')
        {
          result += '\\';
        }
        result += c;
      }
      result += "\"";
      return result;
    }

    void run_command(const std::string& command)
    {
      const int status = std::system(command.c_str());
      if (status != 0)
      {
        throw Roo::RooException("Command failed: " + command);
      }
    }
  } // namespace

  void build_project(const Options& options, const GeneratedProject& project)
  {
    const auto cmake_build_dir = options.build_dir / "build";
    const std::string configure = "cmake -S " + shell_arg(cmake_path(options.build_dir)) +
                                  " -B " + shell_arg(cmake_path(cmake_build_dir)) +
                                  " -DCMAKE_BUILD_TYPE=Release";
    const std::string build = "cmake --build " + shell_arg(cmake_path(cmake_build_dir)) +
                              " --config Release --target " + project.executable_name;
    run_command(configure);
    run_command(build);
  }
} // namespace Rooc

#ifndef ROOC_OPTIONS_H
#define ROOC_OPTIONS_H

#include <filesystem>
#include <string>

namespace Rooc
{
  struct Options
  {
    std::filesystem::path executable_path;
    std::string command;
    std::filesystem::path package_dir;
    std::filesystem::path build_dir;
    std::string executable_name;
  };

  void print_usage();
  void print_help();
  void print_version();
  Options parse_args(int argc, char** argv);
} // namespace Rooc

#endif

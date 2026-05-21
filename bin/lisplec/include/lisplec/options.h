#ifndef LISPLEC_OPTIONS_H
#define LISPLEC_OPTIONS_H

#include <filesystem>
#include <string>

namespace Lisplec
{
  struct Options
  {
    std::string command;
    std::filesystem::path package_dir;
    std::filesystem::path build_dir;
    std::string executable_name;
  };

  void print_usage();
  void print_help();
  void print_version();
  Options parse_args(int argc, char** argv);
} // namespace Lisplec

#endif

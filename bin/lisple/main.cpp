#include <exception>
#include <iostream>
#include <string>

#include <lisple/dir_root_file_system.h>
#include <lisple/runtime.h>

namespace
{
  void print_help()
  {
    std::cout << "Usage: lisple <file>\n";
  }
} // namespace

int main(int argc, char** argv)
{
  if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))
  {
    print_help();
    return 0;
  }

  if (argc != 2)
  {
    print_help();
    return 1;
  }

  try
  {
    Lisple::DirRootFileSystem lisple_fs(".");
    Lisple::Runtime runtime(&lisple_fs);
    runtime.read_file(argv[1]);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

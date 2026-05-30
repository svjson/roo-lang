#include <exception>
#include <iostream>

#include <rooc/builder.h>
#include <rooc/generator.h>
#include <rooc/options.h>
#include <rooc/project.h>

int main(int argc, char** argv)
{
  try
  {
    Rooc::Options options = Rooc::parse_args(argc, argv);
    Rooc::GeneratedProject project = Rooc::prepare_project(options);

    Rooc::generate_project(options, project);
    if (options.command == "build")
    {
      Rooc::build_project(options, project);
    }

    std::cout << "Generated " << project.executable_name << " in "
              << options.build_dir.string() << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    Rooc::print_usage();
    return 1;
  }

  return 0;
}

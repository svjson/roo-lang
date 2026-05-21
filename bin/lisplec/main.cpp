#include <exception>
#include <iostream>

#include <lisplec/builder.h>
#include <lisplec/generator.h>
#include <lisplec/options.h>
#include <lisplec/project.h>

int main(int argc, char** argv)
{
  try
  {
    Lisplec::Options options = Lisplec::parse_args(argc, argv);
    Lisplec::GeneratedProject project = Lisplec::prepare_project(options);

    Lisplec::generate_project(options, project);
    if (options.command == "build")
    {
      Lisplec::build_project(options, project);
    }

    std::cout << "Generated " << project.executable_name << " in "
              << options.build_dir.string() << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    Lisplec::print_usage();
    return 1;
  }

  return 0;
}

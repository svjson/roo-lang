
#include <iostream>

#include <lisple/io/dir_root_file_system.h>
#include <lisple/lang.h>
#include <lisple/runtime.h>
#include <lisple/type.h>

#include <roo-server/server.h>

int main(int, char**)
{
  Lisple::Namespace roo_lang = Lisple::make_language_namespace();
  Lisple::DirRootFileSystem roo_fs(".");
  Lisple::Runtime runtime(&roo_fs);
  runtime.set_call_stack_diagnostics(true);

  Roo::Server::Server server({.application_name = "Roo Stand-alone REPL Server",
                                 .application_version = "0.1",
                                 .port = 8100},
                                runtime);
  auto status = server.start();

  if (status.code == 1)
  {
    while (1)
    {
      server.query_sockets();
    }
  }
  std::cout << status.message << std::endl;

  return 0;
}

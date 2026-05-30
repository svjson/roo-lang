
#include <iostream>

#include <roo/io/dir_root_file_system.h>
#include <roo/lang.h>
#include <roo/runtime.h>
#include <roo/type.h>

#include <roo-server/server.h>

int main(int, char**)
{
  Roo::Namespace roo_lang = Roo::make_language_namespace();
  Roo::DirRootFileSystem roo_fs(".");
  Roo::Runtime runtime(&roo_fs);
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

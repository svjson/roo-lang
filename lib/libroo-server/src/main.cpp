
#include <charconv>
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

#include <roo/io/dir_root_file_system.h>
#include <roo/lang.h>
#include <roo/runtime.h>
#include <roo/type.h>

#include <roo-server/server.h>

namespace
{
  constexpr unsigned int DEFAULT_PORT = 8100;

  void print_usage(std::ostream& output)
  {
    output << "Usage: roo-server [--host <IPv4-address>] [--port <0-65535>]\n"
              "\n"
              "Start a Roo evaluation server. The host defaults to 127.0.0.1;\n"
              "use --host 0.0.0.0 to explicitly accept remote connections.\n"
              "Port 0 asks the operating system to select an available port.\n";
  }

  bool parse_port(std::string_view value, unsigned int& port)
  {
    unsigned int parsed = 0;
    const auto result = std::from_chars(value.data(), value.data() + value.size(), parsed);
    if (result.ec != std::errc{} || result.ptr != value.data() + value.size() ||
        parsed > 65535)
    {
      return false;
    }
    port = parsed;
    return true;
  }
} // namespace

int main(int argc, char** argv)
{
  std::string bind_address = "127.0.0.1";
  unsigned int port = DEFAULT_PORT;
  for (int i = 1; i < argc; ++i)
  {
    const std::string_view argument(argv[i]);
    if (argument == "--help" || argument == "-h")
    {
      print_usage(std::cout);
      return 0;
    }
    if (argument == "--port")
    {
      if (++i >= argc || !parse_port(argv[i], port))
      {
        std::cerr << "roo-server: --port requires a number from 0 to 65535\n";
        return 2;
      }
      continue;
    }
    if (argument == "--host" || argument == "--bind-address")
    {
      if (++i >= argc)
      {
        std::cerr << "roo-server: " << argument << " requires an IPv4 address\n";
        return 2;
      }
      bind_address = argv[i];
      continue;
    }
    constexpr std::string_view port_prefix = "--port=";
    if (argument.starts_with(port_prefix))
    {
      if (!parse_port(argument.substr(port_prefix.size()), port))
      {
        std::cerr << "roo-server: invalid port: " << argument.substr(port_prefix.size())
                  << '\n';
        return 2;
      }
      continue;
    }
    constexpr std::string_view host_prefix = "--host=";
    constexpr std::string_view bind_address_prefix = "--bind-address=";
    if (argument.starts_with(host_prefix))
    {
      bind_address = argument.substr(host_prefix.size());
      continue;
    }
    if (argument.starts_with(bind_address_prefix))
    {
      bind_address = argument.substr(bind_address_prefix.size());
      continue;
    }

    std::cerr << "roo-server: unknown argument: " << argument << '\n';
    print_usage(std::cerr);
    return 2;
  }

  [[maybe_unused]] Roo::Namespace roo_lang = Roo::make_language_namespace();
  Roo::DirRootFileSystem roo_fs(".");
  Roo::Runtime runtime(&roo_fs);
  runtime.set_call_stack_diagnostics(true);

  Roo::Server::Server server({.server_name = "Roo Stand-alone Evaluation Server",
                              .bind_address = bind_address,
                              .port = port},
                             runtime);
  auto status = server.start();
  if (status.code < 0)
  {
    std::cerr << "roo-server: " << status.message << '\n';
    return 1;
  }

  std::cout << "ROO_SERVER_READY host=" << server.address() << " port=" << server.port()
            << std::endl;
  while (true)
  {
    server.query_sockets();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  return 0;
}

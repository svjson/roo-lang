
#include "roo-server/server.h"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#include "roo-server/dispatch.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

namespace Roo::Server
{
  using Roo::Runtime;

  namespace
  {
#ifdef _WIN32
    using NativeSocketHandle = SOCKET;
    constexpr SocketHandle INVALID_SOCKET_HANDLE = static_cast<SocketHandle>(-1);
    constexpr short SOCKET_READ_EVENT = POLLRDNORM;

    NativeSocketHandle to_native_socket(SocketHandle socket)
    {
      return static_cast<NativeSocketHandle>(socket);
    }

    SocketHandle from_native_socket(NativeSocketHandle socket)
    {
      return static_cast<SocketHandle>(socket);
    }

    std::string socket_error_message()
    {
      return "socket error: " + std::to_string(WSAGetLastError());
    }

    void close_socket(SocketHandle socket)
    {
      closesocket(to_native_socket(socket));
    }

    void cleanup_socket_runtime(bool& socket_runtime_started)
    {
      if (socket_runtime_started)
      {
        WSACleanup();
        socket_runtime_started = false;
      }
    }

    int poll_sockets(std::vector<SocketDescriptor>& socket_descriptors)
    {
      std::vector<WSAPOLLFD> native_descriptors;
      native_descriptors.reserve(socket_descriptors.size());
      for (const auto& descriptor : socket_descriptors)
      {
        native_descriptors.push_back(
          {to_native_socket(descriptor.fd), descriptor.events, descriptor.revents});
      }

      const int result =
        WSAPoll(native_descriptors.data(), static_cast<ULONG>(native_descriptors.size()), 0);
      for (size_t i = 0; i < socket_descriptors.size(); ++i)
      {
        socket_descriptors[i].revents = native_descriptors[i].revents;
      }
      return result;
    }
#else
    using NativeSocketHandle = int;
    constexpr SocketHandle INVALID_SOCKET_HANDLE = static_cast<SocketHandle>(-1);
    constexpr short SOCKET_READ_EVENT = POLLIN;

    NativeSocketHandle to_native_socket(SocketHandle socket)
    {
      return static_cast<NativeSocketHandle>(socket);
    }

    SocketHandle from_native_socket(NativeSocketHandle socket)
    {
      return static_cast<SocketHandle>(socket);
    }

    std::string socket_error_message()
    {
      return std::strerror(errno);
    }

    void close_socket(SocketHandle socket)
    {
      close(to_native_socket(socket));
    }

    void cleanup_socket_runtime(bool&) {}

    int poll_sockets(std::vector<SocketDescriptor>& socket_descriptors)
    {
      std::vector<pollfd> native_descriptors;
      native_descriptors.reserve(socket_descriptors.size());
      for (const auto& descriptor : socket_descriptors)
      {
        native_descriptors.push_back(
          {to_native_socket(descriptor.fd), descriptor.events, descriptor.revents});
      }

      const int result = poll(native_descriptors.data(), native_descriptors.size(), 0);
      for (size_t i = 0; i < socket_descriptors.size(); ++i)
      {
        socket_descriptors[i].revents = native_descriptors[i].revents;
      }
      return result;
    }
#endif
  } // namespace

  void set_socket_non_blocking(SocketHandle socket_fd)
  {
#ifdef _WIN32
    u_long mode = 1;
    if (ioctlsocket(to_native_socket(socket_fd), FIONBIO, &mode) != 0)
    {
      std::cout << "Setting non-blocking socket mode for socket " << socket_fd << " failed."
                << std::endl;
    }
#else
    int flags = fcntl(to_native_socket(socket_fd), F_GETFL, 0);
    if (flags == -1)
    {
      std::cout << "Getting socket flags for socket " << socket_fd << " failed."
                << std::endl;
    }

    if (fcntl(to_native_socket(socket_fd), F_SETFL, flags |= O_NONBLOCK) == -1)
    {
      std::cout << "Setting O_NONBLOCK socket flag for socket " << socket_fd << " failed."
                << std::endl;
    }
#endif
  }

  Server::Server(const ServerConfig& config, Runtime& runtime)
    : config(config)
    , dispatcher(this->config, runtime)
  {
  }

  ServerStatus Server::start()
  {
#ifdef _WIN32
    WSADATA wsa_data;
    if (!socket_runtime_started && WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
      return ServerStatus{-1, "Could not initialize Winsock."};
    }
    socket_runtime_started = true;
#endif

    NativeSocketHandle native_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    SocketHandle server_socket = from_native_socket(native_server_socket);
    int opt = 1;
    sockaddr_in address{};
    if (native_server_socket == static_cast<NativeSocketHandle>(-1))
    {
      cleanup_socket_runtime(socket_runtime_started);
      return ServerStatus{-1, "Could not open socket: " + socket_error_message()};
    }

    if (setsockopt(to_native_socket(server_socket),
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   reinterpret_cast<const char*>(&opt),
                   static_cast<int>(sizeof(opt))))
    {
      close_socket(server_socket);
      cleanup_socket_runtime(socket_runtime_started);
      return ServerStatus{-1, "Could not configure socket."};
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(config.port);

    if (bind(to_native_socket(server_socket), (sockaddr*)&address, sizeof(address)) < 0)
    {
      close_socket(server_socket);
      cleanup_socket_runtime(socket_runtime_started);
      return ServerStatus{-1,
                          "Could not bind socket to port: " + std::to_string(config.port)};
    }

    if (listen(to_native_socket(server_socket), 3) < 0)
    {
      close_socket(server_socket);
      cleanup_socket_runtime(socket_runtime_started);
      return ServerStatus{-1, "Listening for connections failed after binding socket."};
    }

    set_socket_non_blocking(server_socket);
    socket_descriptors.push_back({server_socket, SOCKET_READ_EVENT, 0});

    //// Temp
    std::cout << "Server listening on port " << config.port << std::endl;

    return ServerStatus{1, "Started"};
  }

  ServerStatus Server::shutdown()
  {
    if (!socket_descriptors.empty() && socket_descriptors[0].fd != INVALID_SOCKET_HANDLE)
    {
      for (auto& descriptor : socket_descriptors)
      {
        if (descriptor.fd != INVALID_SOCKET_HANDLE)
        {
          close_socket(descriptor.fd);
          descriptor.fd = INVALID_SOCKET_HANDLE;
        }
      }
      socket_descriptors.clear();
      cleanup_socket_runtime(socket_runtime_started);

      return ServerStatus{1, "Shutdown"};
    }

    return ServerStatus{1, "Not Started"};
  }

  void Server::query_sockets()
  {
    if (socket_descriptors.empty())
    {
      return;
    }

    int poll_count = poll_sockets(socket_descriptors);
    if (poll_count < 0)
    {
      std::cout << "Error! Polling failed." << std::endl;
    }

    if (socket_descriptors[0].revents & SOCKET_READ_EVENT)
    {
      NativeSocketHandle native_new_client_sock =
        accept(to_native_socket(socket_descriptors[0].fd), nullptr, nullptr);
      if (native_new_client_sock != static_cast<NativeSocketHandle>(-1))
      {
        SocketHandle new_client_sock = from_native_socket(native_new_client_sock);
        set_socket_non_blocking(new_client_sock);
        socket_descriptors.push_back({new_client_sock, SOCKET_READ_EVENT, 0});
      }
    }

    for (size_t i = 1; i < socket_descriptors.size(); i++)
    {
      if (socket_descriptors[i].revents & SOCKET_READ_EVENT)
      {
        accept_request(socket_descriptors[i].fd);
      }
    }
  }

  void Server::accept_request(SocketHandle socket)
  {
    int valread = 1;
    char buffer[1024] = {0};

    std::string raw_message;

    while (valread > 0)
    {
      valread = recv(to_native_socket(socket), buffer, static_cast<int>(sizeof(buffer)), 0);

      if (valread > 0)
      {
        raw_message.append(buffer, static_cast<size_t>(valread));
        memset(buffer, 0, sizeof(buffer));
      }
    }

    std::cout << "<---------------" << std::endl;
    std::cout << raw_message << std::endl;
    auto message = message_parser.parse_message(raw_message);
    std::string response;

    if (auto* err = std::get_if<Error>(&message))
    {
      std::cout << "err: " << err->msg << std::endl;
      response = MessageBuilder::build_error_response(*err).encode();
    }
    else
    {
      std::cout << "cmd: " << std::get<Message>(message).get_block(_CMD).get_property(_ID)
                << std::endl;

      auto result = dispatcher.dispatch(std::get<Message>(message));

      if (auto* err = std::get_if<Error>(&result))
        response = MessageBuilder::build_error_response(*err).encode();
      else
        response = std::get<Response>(result).encode();
    }
    std::cout << response << std::endl;
    std::cout << "--------------->" << std::endl;
    send(to_native_socket(socket), response.c_str(), static_cast<int>(response.length()), 0);
  }
} // namespace Roo::Server

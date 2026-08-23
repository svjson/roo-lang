
#include "roo-server/server.h"

#include <cerrno>
#include <cstddef>
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
    constexpr short SOCKET_WRITE_EVENT = POLLWRNORM;
    constexpr short SOCKET_ERROR_EVENTS = POLLERR | POLLHUP | POLLNVAL;
    constexpr int SOCKET_SEND_FLAGS = 0;
    using SocketLength = int;

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

    bool socket_would_block()
    {
      return WSAGetLastError() == WSAEWOULDBLOCK;
    }

    bool parse_ipv4_address(const std::string& value, in_addr& address)
    {
      return InetPtonA(AF_INET, value.c_str(), &address) == 1;
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
          {to_native_socket(descriptor.fd), descriptor.events, 0});
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
    constexpr short SOCKET_WRITE_EVENT = POLLOUT;
    constexpr short SOCKET_ERROR_EVENTS = POLLERR | POLLHUP | POLLNVAL;
#ifdef MSG_NOSIGNAL
    constexpr int SOCKET_SEND_FLAGS = MSG_NOSIGNAL;
#else
    constexpr int SOCKET_SEND_FLAGS = 0;
#endif
    using SocketLength = socklen_t;

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

    bool socket_would_block()
    {
      return errno == EAGAIN || errno == EWOULDBLOCK;
    }

    bool parse_ipv4_address(const std::string& value, in_addr& address)
    {
      return inet_pton(AF_INET, value.c_str(), &address) == 1;
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
          {to_native_socket(descriptor.fd), descriptor.events, 0});
      }

      const int result = poll(native_descriptors.data(), native_descriptors.size(), 0);
      for (size_t i = 0; i < socket_descriptors.size(); ++i)
      {
        socket_descriptors[i].revents = native_descriptors[i].revents;
      }
      return result;
    }
#endif

    constexpr size_t MAX_PENDING_INPUT_SIZE = 16 * 1024 * 1024;
    const std::string MESSAGE_TERMINATOR = "/MSG\x1E";
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

  Server::~Server()
  {
    shutdown();
  }

  ServerStatus Server::start()
  {
    if (!socket_descriptors.empty())
    {
      return ServerStatus{1, "Already started"};
    }
    if (config.port > 65535)
    {
      return ServerStatus{-1, "Invalid TCP port: " + std::to_string(config.port)};
    }

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
    if (!parse_ipv4_address(config.bind_address, address.sin_addr))
    {
      close_socket(server_socket);
      cleanup_socket_runtime(socket_runtime_started);
      return ServerStatus{-1, "Invalid IPv4 bind address: " + config.bind_address};
    }
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

    sockaddr_in bound_address{};
    SocketLength bound_address_length = sizeof(bound_address);
    if (getsockname(to_native_socket(server_socket),
                    reinterpret_cast<sockaddr*>(&bound_address),
                    &bound_address_length) < 0)
    {
      close_socket(server_socket);
      cleanup_socket_runtime(socket_runtime_started);
      return ServerStatus{-1, "Could not inspect bound socket: " + socket_error_message()};
    }
    bound_port = ntohs(bound_address.sin_port);

    set_socket_non_blocking(server_socket);
    socket_descriptors.push_back({server_socket, SOCKET_READ_EVENT, 0, "", ""});

    return ServerStatus{
      1,
      "Listening on " + config.bind_address + ":" + std::to_string(bound_port)};
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
      bound_port = 0;
      cleanup_socket_runtime(socket_runtime_started);

      return ServerStatus{1, "Shutdown"};
    }

    return ServerStatus{1, "Not Started"};
  }

  unsigned int Server::port() const
  {
    return bound_port;
  }

  const std::string& Server::address() const
  {
    return config.bind_address;
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
      return;
    }
    if (poll_count == 0)
    {
      return;
    }

    if (socket_descriptors[0].revents & SOCKET_READ_EVENT)
    {
      NativeSocketHandle native_new_client_sock =
        accept(to_native_socket(socket_descriptors[0].fd), nullptr, nullptr);
      if (native_new_client_sock != static_cast<NativeSocketHandle>(-1))
      {
        SocketHandle new_client_sock = from_native_socket(native_new_client_sock);
        set_socket_non_blocking(new_client_sock);
        socket_descriptors.push_back({new_client_sock, SOCKET_READ_EVENT, 0, "", ""});
      }
    }

    for (size_t i = 1; i < socket_descriptors.size();)
    {
      auto& descriptor = socket_descriptors[i];
      bool connected = !(descriptor.revents & SOCKET_ERROR_EVENTS);

      if (connected && (descriptor.revents & SOCKET_READ_EVENT))
      {
        connected = receive_requests(descriptor);
      }
      if (connected && !descriptor.output.empty())
      {
        connected = flush_responses(descriptor);
      }

      if (!connected)
      {
        close_socket(descriptor.fd);
        socket_descriptors.erase(socket_descriptors.begin() +
                                 static_cast<std::ptrdiff_t>(i));
      }
      else
      {
        descriptor.events = SOCKET_READ_EVENT;
        if (!descriptor.output.empty())
        {
          descriptor.events |= SOCKET_WRITE_EVENT;
        }
        ++i;
      }
    }
  }

  bool Server::receive_requests(SocketDescriptor& socket)
  {
    char buffer[4096];
    while (true)
    {
      const int received =
        recv(to_native_socket(socket.fd), buffer, static_cast<int>(sizeof(buffer)), 0);
      if (received > 0)
      {
        socket.input.append(buffer, static_cast<size_t>(received));
      }
      else if (received == 0)
      {
        return false;
      }
      else if (socket_would_block())
      {
        break;
      }
      else
      {
        return false;
      }
    }

    size_t message_end;
    while ((message_end = socket.input.find(MESSAGE_TERMINATOR)) != std::string::npos)
    {
      message_end += MESSAGE_TERMINATOR.size();
      socket.output += dispatch_message(socket.input.substr(0, message_end));
      socket.input.erase(0, message_end);
    }
    return socket.input.size() <= MAX_PENDING_INPUT_SIZE;
  }

  bool Server::flush_responses(SocketDescriptor& socket)
  {
    while (!socket.output.empty())
    {
      const int sent = send(to_native_socket(socket.fd),
                            socket.output.data(),
                            static_cast<int>(socket.output.size()),
                            SOCKET_SEND_FLAGS);
      if (sent > 0)
      {
        socket.output.erase(0, static_cast<size_t>(sent));
      }
      else if (sent < 0 && socket_would_block())
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    return true;
  }

  std::string Server::dispatch_message(const std::string& raw_message)
  {
    auto message = message_parser.parse_message(raw_message);

    if (auto* err = std::get_if<Error>(&message))
    {
      return MessageBuilder::build_error_response(*err).encode();
    }

    auto result = dispatcher.dispatch(std::get<Message>(message));

    if (auto* err = std::get_if<Error>(&result))
    {
      return MessageBuilder::build_error_response(*err).encode();
    }
    return std::get<Response>(result).encode();
  }
} // namespace Roo::Server

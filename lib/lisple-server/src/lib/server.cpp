
#include "lisple-server/server.h"

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "lisple-server/dispatch.h"

namespace Lisple
{
  namespace Server
  {
    void set_socket_non_blocking(int socket_fd)
    {
      int flags = fcntl(socket_fd, F_GETFL, 0);
      if (flags == -1)
      {
        std::cout << "Getting socket flags for socket " << socket_fd << " failed." << std::endl;
      }

      if (fcntl(socket_fd, F_SETFL, flags |= O_NONBLOCK) == -1)
      {
        std::cout << "Setting O_NONBLOCK socket flag for socket " << socket_fd << " failed." << std::endl;
      }
    }

    Server::Server(const ServerConfig& config, Runtime& runtime)
      : config(config)
      , dispatcher(this->config, runtime)
    {}

    ServerStatus Server::start()
    {
      int server_socket = socket(AF_INET, SOCK_STREAM, 0);
      int opt = 1;
      sockaddr_in address;
      if (server_socket == 0) return ServerStatus { -1, "Could not open socket." };

      if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
      {
        return ServerStatus { -1, "Could not configure socket." };
      }

      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY;
      address.sin_port = htons(config.port);

      if (bind(server_socket, (sockaddr*) &address, sizeof(address)) < 0)
      {
        return ServerStatus { -1, "Could not bind socket to port: " + std::to_string(config.port) };
      }

      if (listen(server_socket, 3) < 0)
      {
        return ServerStatus { -1, "Listening for connections failed after binding socket." };
      }

      set_socket_non_blocking(server_socket);
      socket_descriptors.push_back({ server_socket, POLLIN, 0 });

      //// Temp
      std::cout << "Server listening on port " << config.port << std::endl;

      return ServerStatus { 1, "Started" };
    }

    ServerStatus Server::shutdown()
    {
      if (socket_descriptors[0].fd > 0) {
#ifdef _WIN32
        closesocket(server_socket);
        WSACleanup();  // Cleanup Winsock on Windows
#else
        close(socket_descriptors[0].fd);
#endif
        socket_descriptors[0].fd = 0;

        return ServerStatus { 1, "Shutdown" };
      }

      return ServerStatus { 1, "Not Started" };
    }

    void Server::query_sockets()
    {
      int poll_count = poll(socket_descriptors.data(), socket_descriptors.size(), 0);
      if (poll_count < 0)
      {
        std::cout << "Error! Polling failed." << std::endl;
      }

      if (socket_descriptors[0].revents & POLLIN)
      {
        int new_client_sock = accept(socket_descriptors[0].fd, nullptr, nullptr);
        if (new_client_sock > 0)
        {
          set_socket_non_blocking(new_client_sock);
          socket_descriptors.push_back({ new_client_sock, POLLIN, 0 });
        }
      }

      for (size_t i=1; i < socket_descriptors.size(); i++)
      {
        if (socket_descriptors[i].revents & POLLIN)
        {
          accept_request(socket_descriptors[i].fd);
        }
      }
    }

    void Server::accept_request(int socket)
    {
      int valread = 1;
      char buffer[1024] = {0};

      std::string raw_message;

      while (valread > 0)
      {
        valread = recv(socket, buffer, sizeof(buffer), 0);

        if (valread > 0)
        {
          raw_message += buffer;
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
        std::cout << "cmd: " << std::get<Message>(message)
          .get_block(_CMD)
          .get_property(_ID) << std::endl;

        auto result = dispatcher.dispatch(std::get<Message>(message));

        if (auto* err = std::get_if<Error>(&result))
          response = MessageBuilder::build_error_response(*err).encode();
        else
          response = std::get<Response>(result).encode();
      }
      std::cout << response << std::endl;
      std::cout << "--------------->" << std::endl;
      send(socket, response.c_str(), response.length(), 0);
    }
  }
}

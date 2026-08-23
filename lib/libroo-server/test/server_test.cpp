#include <string>
#include <variant>

#include <roo/io/dir_root_file_system.h>
#include <roo/lang.h>
#include <roo/runtime.h>

#include <gtest/gtest.h>
#include <roo-server/message.h>
#include <roo-server/server.h>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

namespace
{
#ifdef _WIN32
  using ClientSocket = SOCKET;
  constexpr ClientSocket INVALID_CLIENT_SOCKET = INVALID_SOCKET;

  void close_client_socket(ClientSocket socket)
  {
    closesocket(socket);
  }

  void set_receive_timeout(ClientSocket socket)
  {
    DWORD timeout = 1000;
    setsockopt(socket,
               SOL_SOCKET,
               SO_RCVTIMEO,
               reinterpret_cast<const char*>(&timeout),
               static_cast<int>(sizeof(timeout)));
  }
#else
  using ClientSocket = int;
  constexpr ClientSocket INVALID_CLIENT_SOCKET = -1;

  void close_client_socket(ClientSocket socket)
  {
    close(socket);
  }

  void set_receive_timeout(ClientSocket socket)
  {
    timeval timeout{1, 0};
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
  }
#endif

  ClientSocket connect_to(unsigned int port)
  {
    const ClientSocket socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket == INVALID_CLIENT_SOCKET)
    {
      return INVALID_CLIENT_SOCKET;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(static_cast<uint16_t>(port));
    if (connect(socket,
                reinterpret_cast<const sockaddr*>(&address),
                static_cast<int>(sizeof(address))) != 0)
    {
      close_client_socket(socket);
      return INVALID_CLIENT_SOCKET;
    }
    set_receive_timeout(socket);
    return socket;
  }

  bool send_all(ClientSocket socket, const std::string& data)
  {
    size_t sent = 0;
    while (sent < data.size())
    {
      const int result =
        send(socket, data.data() + sent, static_cast<int>(data.size() - sent), 0);
      if (result <= 0)
      {
        return false;
      }
      sent += static_cast<size_t>(result);
    }
    return true;
  }
} // namespace

TEST(Server, defaults_to_loopback_and_reports_an_ephemeral_port)
{
  [[maybe_unused]] Roo::Namespace roo_lang = Roo::make_language_namespace();
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::Server server({.port = 0}, runtime);

  const auto status = server.start();

  ASSERT_GT(status.code, 0) << status.message;
  EXPECT_EQ(server.address(), "127.0.0.1");
  EXPECT_GT(server.port(), 0U);
}

TEST(Server, rejects_an_invalid_bind_address)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::Server server({.bind_address = "not-an-address", .port = 0}, runtime);

  const auto status = server.start();

  EXPECT_LT(status.code, 0);
  EXPECT_EQ(server.port(), 0U);
}

TEST(Server, rejects_a_port_outside_the_tcp_range)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::Server server({.port = 65536}, runtime);

  const auto status = server.start();

  EXPECT_LT(status.code, 0);
  EXPECT_EQ(server.port(), 0U);
}

TEST(Server, buffers_a_request_until_the_complete_message_arrives)
{
  [[maybe_unused]] Roo::Namespace roo_lang = Roo::make_language_namespace();
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::Server server({.port = 0}, runtime);
  ASSERT_GT(server.start().code, 0);

  const ClientSocket client = connect_to(server.port());
  ASSERT_NE(client, INVALID_CLIENT_SOCKET);
  server.query_sockets();

  const std::string request =
    "!MSG\x1E!CMD\x1E@ID=eval-sexp\x1E@NS=user\x1E@BODY=(+ 1 2)\x1E/CMD\x1E/MSG\x1E";
  const size_t split = request.size() / 2;
  ASSERT_TRUE(send_all(client, request.substr(0, split)));
  server.query_sockets();
  ASSERT_TRUE(send_all(client, request.substr(split)));
  server.query_sockets();

  char response_buffer[4096];
  const int received =
    recv(client, response_buffer, static_cast<int>(sizeof(response_buffer)), 0);
  ASSERT_GT(received, 0);
  close_client_socket(client);

  Roo::Server::MessageParser parser;
  auto parsed =
    parser.parse_message(std::string(response_buffer, static_cast<size_t>(received)));
  ASSERT_TRUE(std::holds_alternative<Roo::Server::Message>(parsed));
  const auto& response =
    std::get<Roo::Server::Message>(parsed).get_block(Roo::Server::_RESP);
  EXPECT_EQ(response.get_property(Roo::Server::_ID), "eval-result");
  EXPECT_EQ(response.get_property(Roo::Server::_BODY), "3");
}

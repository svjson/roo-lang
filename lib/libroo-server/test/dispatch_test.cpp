#include <string>
#include <utility>
#include <variant>

#include <roo/io/dir_root_file_system.h>
#include <roo/runtime.h>

#include <gtest/gtest.h>
#include <roo-server/dispatch.h>
#include <roo-server/message.h>
#include <roo-server/server.h>

namespace
{
  Roo::Server::Message parse_message(const std::string& source)
  {
    Roo::Server::MessageParser parser;
    auto parsed = parser.parse_message(source);
    EXPECT_TRUE(std::holds_alternative<Roo::Server::Message>(parsed));
    return std::get<Roo::Server::Message>(std::move(parsed));
  }
} // namespace

TEST(Dispatcher, standalone_server_info_uses_the_server_version)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::ServerConfig config{
    .server_name = "Test Roo Server",
    .server_version = "1.2.3",
  };
  Roo::Server::Dispatcher dispatcher(config, runtime);

  auto result = dispatcher.dispatch(
    parse_message("!MSG\x1E!CMD\x1E@ID=server-info\x1E/CMD\x1E/MSG\x1E"));

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Response>(result));
  const auto& response =
    std::get<Roo::Server::Response>(result).get_block(Roo::Server::_RESP);
  EXPECT_EQ(response.get_property(Roo::Server::_BODY), "Test Roo Server 1.2.3");
}

TEST(Dispatcher, eval_sexp_reports_a_missing_namespace)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::ServerConfig config;
  Roo::Server::Dispatcher dispatcher(config, runtime);

  auto result = dispatcher.dispatch(
    parse_message("!MSG\x1E!CMD\x1E@ID=eval-sexp\x1E@BODY=(+ 1 2)\x1E/CMD\x1E/MSG\x1E"));

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Error>(result));
  EXPECT_EQ(std::get<Roo::Server::Error>(result).msg, "CMD 'eval-sexp' requires @NS");
}

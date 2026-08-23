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

  std::variant<Roo::Server::Response, Roo::Server::Error> eval_sexp(
    Roo::Server::Dispatcher& dispatcher,
    const std::string& source,
    const std::string& namespace_name = "user")
  {
    return dispatcher.dispatch(
      parse_message("!MSG\x1E!CMD\x1E@ID=eval-sexp\x1E@NS=" + namespace_name +
                    "\x1E@BODY=" + source + "\x1E/CMD\x1E/MSG\x1E"));
  }

  std::string response_body(
    const std::variant<Roo::Server::Response, Roo::Server::Error>& result)
  {
    EXPECT_TRUE(std::holds_alternative<Roo::Server::Response>(result));
    return std::get<Roo::Server::Response>(result)
      .get_block(Roo::Server::_RESP)
      .get_property(Roo::Server::_BODY);
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

TEST(Dispatcher, eval_sexp_uses_the_requested_namespace_and_restores_the_previous_one)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::ServerConfig config;
  Roo::Server::Dispatcher dispatcher(config, runtime);

  auto defined =
    eval_sexp(dispatcher, "(defun advance-flow [] 1)", "campaign-kit.backend.flow");

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Response>(defined));
  EXPECT_EQ(response_body(defined), "#'campaign-kit.backend.flow/advance-flow");
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "user");

  auto failed = eval_sexp(dispatcher, "unknown-value", "campaign-kit.backend.flow");

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Error>(failed));
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "user");
}

TEST(Dispatcher, eval_sexp_replaces_a_single_top_level_definition)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::ServerConfig config;
  Roo::Server::Dispatcher dispatcher(config, runtime);

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Response>(
    eval_sexp(dispatcher, "(defun answer [] 41)")));
  ASSERT_TRUE(std::holds_alternative<Roo::Server::Response>(
    eval_sexp(dispatcher, "(defun answer [] 42)")));
  EXPECT_EQ(response_body(eval_sexp(dispatcher, "(answer)")), "42");

  ASSERT_TRUE(
    std::holds_alternative<Roo::Server::Response>(eval_sexp(dispatcher, "(def value 1)")));
  ASSERT_TRUE(
    std::holds_alternative<Roo::Server::Response>(eval_sexp(dispatcher, "(def value 2)")));
  EXPECT_EQ(response_body(eval_sexp(dispatcher, "value")), "2");
}

TEST(Dispatcher, eval_sexp_restores_a_definition_when_replacement_fails)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::ServerConfig config;
  Roo::Server::Dispatcher dispatcher(config, runtime);

  ASSERT_TRUE(
    std::holds_alternative<Roo::Server::Response>(eval_sexp(dispatcher, "(def answer 41)")));
  auto failed = eval_sexp(dispatcher, "(def answer unknown-value)");

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Error>(failed));
  EXPECT_EQ(response_body(eval_sexp(dispatcher, "answer")), "41");

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Response>(
    eval_sexp(dispatcher, "(defun function-answer [] 41)")));
  auto failed_function = eval_sexp(dispatcher, "(defun function-answer [1] 42)");

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Error>(failed_function));
  EXPECT_EQ(response_body(eval_sexp(dispatcher, "(function-answer)")), "41");
}

TEST(Dispatcher, eval_sexp_does_not_replace_definitions_in_multi_form_source)
{
  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);
  Roo::Server::ServerConfig config;
  Roo::Server::Dispatcher dispatcher(config, runtime);

  ASSERT_TRUE(
    std::holds_alternative<Roo::Server::Response>(eval_sexp(dispatcher, "(def answer 41)")));
  auto failed = eval_sexp(dispatcher, "(def answer 42) (def another 1)");

  ASSERT_TRUE(std::holds_alternative<Roo::Server::Error>(failed));
  EXPECT_EQ(response_body(eval_sexp(dispatcher, "answer")), "41");
}

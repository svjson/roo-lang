
#include <gtest/gtest.h>
#include <variant>

#include "message.h"

TEST(MessageParser, parse_eval_sexp)
{
  const std::string input = "!MSG\x1E!CMD\x1E@ID=eval-sexp\x1E@NS=user\x1E@BODY={:lisple \"code\" :goes \"here\"}\x1E/CMD\x1E/MSG\x1E";
  Lisple::Server::MessageParser parser;

  auto result = parser.parse_message(input);
  if (auto* err = std::get_if<Lisple::Server::Error>(&result))
  {
    std::cout << err->msg << std::endl;
  }
  ASSERT_TRUE(std::holds_alternative<Lisple::Server::Message>(result));

  Lisple::Server::Message& message = std::get<Lisple::Server::Message>(result);

  ASSERT_TRUE(message.has_block("CMD")) << message.encode();
  const Lisple::Server::Block& cmd = message.get_block("CMD");

  ASSERT_TRUE(cmd.has_property("ID"));
  EXPECT_EQ(cmd.get_property("ID"), "eval-sexp");

  ASSERT_TRUE(cmd.has_property("NS"));
  EXPECT_EQ(cmd.get_property("NS"), "user");

  ASSERT_TRUE(cmd.has_property("BODY"));
  EXPECT_EQ(cmd.get_property("BODY"), "{:lisple \"code\" :goes \"here\"}");
}

#include <lisple/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using StringCaseFunction = LispleTest::RuntimeTestFixture;

TEST_F(StringCaseFunction, capitalize_capitalizes_all_words_by_default)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize "string"))"), *Lisple::Value::string("String"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words"))"),
            *Lisple::Value::string("String Of Words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "sTRing    oF words"))"),
            *Lisple::Value::string("String    Of Words"));
}

TEST_F(StringCaseFunction, capitalize_can_limit_number_of_words)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words" 0))"),
            *Lisple::Value::string("string of words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words" 1))"),
            *Lisple::Value::string("String of words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words" 2))"),
            *Lisple::Value::string("String Of words"));
}

TEST_F(StringCaseFunction, capitalize_can_capitalize_inclusive_word_range)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize "a brown fox ate a bug today" [2 4]))"),
            *Lisple::Value::string("a brown Fox Ate A bug today"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "one two three" [1 1]))"),
            *Lisple::Value::string("one Two three"));
}

TEST_F(StringCaseFunction, capitalize_first_capitalizes_only_first_word)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize-first "string of words"))"),
            *Lisple::Value::string("String of words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize-first "  sTRing of words"))"),
            *Lisple::Value::string("  String of words"));
}

TEST_F(StringCaseFunction, capitalize_rejects_invalid_count_and_range_arguments)
{
  EXPECT_THROW(runtime.eval(R"((capitalize "string" -1))"), Lisple::LispleException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" 1.5))"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [2]))"), Lisple::LispleException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [2 1]))"), Lisple::LispleException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [-1 1]))"), Lisple::LispleException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [0 1.5]))"), Lisple::TypeError);
}

TEST_F(StringCaseFunction, kebab_case_converts_common_word_separators)
{
  EXPECT_EQ(*runtime.eval(R"((kebab-case "Player Ship State"))"),
            *Lisple::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "player_ship_state"))"),
            *Lisple::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "player.ship/state"))"),
            *Lisple::Value::string("player-ship-state"));
}

TEST_F(StringCaseFunction, kebab_case_splits_camel_and_pascal_case)
{
  EXPECT_EQ(*runtime.eval(R"((kebab-case "playerShipState"))"),
            *Lisple::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "PlayerShipState"))"),
            *Lisple::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "HTTPRequestBody"))"),
            *Lisple::Value::string("http-request-body"));
}

TEST_F(StringCaseFunction, pascal_case_converts_common_word_separators)
{
  EXPECT_EQ(*runtime.eval(R"((pascal-case "player ship state"))"),
            *Lisple::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "player-ship-state"))"),
            *Lisple::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "player_ship_state"))"),
            *Lisple::Value::string("PlayerShipState"));
}

TEST_F(StringCaseFunction, pascal_case_splits_camel_and_pascal_case)
{
  EXPECT_EQ(*runtime.eval(R"((pascal-case "playerShipState"))"),
            *Lisple::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "PlayerShipState"))"),
            *Lisple::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "HTTPRequestBody"))"),
            *Lisple::Value::string("HttpRequestBody"));
}

TEST_F(StringCaseFunction, camel_case_converts_common_word_separators)
{
  EXPECT_EQ(*runtime.eval(R"((camel-case "player ship state"))"),
            *Lisple::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "player-ship-state"))"),
            *Lisple::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "player_ship_state"))"),
            *Lisple::Value::string("playerShipState"));
}

TEST_F(StringCaseFunction, camel_case_splits_camel_and_pascal_case)
{
  EXPECT_EQ(*runtime.eval(R"((camel-case "playerShipState"))"),
            *Lisple::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "PlayerShipState"))"),
            *Lisple::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "HTTPRequestBody"))"),
            *Lisple::Value::string("httpRequestBody"));
}

TEST_F(StringCaseFunction, string_case_functions_handle_empty_and_separator_only_strings)
{
  EXPECT_EQ(*runtime.eval(R"((kebab-case ""))"), *Lisple::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((pascal-case " - _ / "))"), *Lisple::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((camel-case " - _ / "))"), *Lisple::Value::string(""));
}

TEST_F(StringCaseFunction, string_case_functions_require_string_argument)
{
  EXPECT_THROW(runtime.eval("(kebab-case :not-a-string)"), Lisple::InvocationException);
  EXPECT_THROW(runtime.eval("(pascal-case :not-a-string)"), Lisple::InvocationException);
  EXPECT_THROW(runtime.eval("(camel-case :not-a-string)"), Lisple::InvocationException);
  EXPECT_THROW(runtime.eval("(capitalize :not-a-string)"), Lisple::InvocationException);
  EXPECT_THROW(runtime.eval("(capitalize-first :not-a-string)"),
               Lisple::InvocationException);
}

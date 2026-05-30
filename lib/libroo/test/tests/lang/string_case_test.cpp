#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/exception.h>

using StringCaseFunction = RooTest::RuntimeTestFixture;

TEST_F(StringCaseFunction, capitalize_capitalizes_all_words_by_default)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize "string"))"), *Roo::Value::string("String"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words"))"),
            *Roo::Value::string("String Of Words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "sTRing    oF words"))"),
            *Roo::Value::string("String    Of Words"));
}

TEST_F(StringCaseFunction, capitalize_can_limit_number_of_words)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words" 0))"),
            *Roo::Value::string("string of words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words" 1))"),
            *Roo::Value::string("String of words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "string of words" 2))"),
            *Roo::Value::string("String Of words"));
}

TEST_F(StringCaseFunction, capitalize_can_capitalize_inclusive_word_range)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize "a brown fox ate a bug today" [2 4]))"),
            *Roo::Value::string("a brown Fox Ate A bug today"));
  EXPECT_EQ(*runtime.eval(R"((capitalize "one two three" [1 1]))"),
            *Roo::Value::string("one Two three"));
}

TEST_F(StringCaseFunction, capitalize_first_capitalizes_only_first_word)
{
  EXPECT_EQ(*runtime.eval(R"((capitalize-first "string of words"))"),
            *Roo::Value::string("String of words"));
  EXPECT_EQ(*runtime.eval(R"((capitalize-first "  sTRing of words"))"),
            *Roo::Value::string("  String of words"));
}

TEST_F(StringCaseFunction, capitalize_rejects_invalid_count_and_range_arguments)
{
  EXPECT_THROW(runtime.eval(R"((capitalize "string" -1))"), Roo::RooException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" 1.5))"), Roo::TypeError);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [2]))"), Roo::RooException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [2 1]))"), Roo::RooException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [-1 1]))"), Roo::RooException);
  EXPECT_THROW(runtime.eval(R"((capitalize "string" [0 1.5]))"), Roo::TypeError);
}

TEST_F(StringCaseFunction, kebab_case_converts_common_word_separators)
{
  EXPECT_EQ(*runtime.eval(R"((kebab-case "Player Ship State"))"),
            *Roo::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "player_ship_state"))"),
            *Roo::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "player.ship/state"))"),
            *Roo::Value::string("player-ship-state"));
}

TEST_F(StringCaseFunction, kebab_case_splits_camel_and_pascal_case)
{
  EXPECT_EQ(*runtime.eval(R"((kebab-case "playerShipState"))"),
            *Roo::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "PlayerShipState"))"),
            *Roo::Value::string("player-ship-state"));
  EXPECT_EQ(*runtime.eval(R"((kebab-case "HTTPRequestBody"))"),
            *Roo::Value::string("http-request-body"));
}

TEST_F(StringCaseFunction, pascal_case_converts_common_word_separators)
{
  EXPECT_EQ(*runtime.eval(R"((pascal-case "player ship state"))"),
            *Roo::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "player-ship-state"))"),
            *Roo::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "player_ship_state"))"),
            *Roo::Value::string("PlayerShipState"));
}

TEST_F(StringCaseFunction, pascal_case_splits_camel_and_pascal_case)
{
  EXPECT_EQ(*runtime.eval(R"((pascal-case "playerShipState"))"),
            *Roo::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "PlayerShipState"))"),
            *Roo::Value::string("PlayerShipState"));
  EXPECT_EQ(*runtime.eval(R"((pascal-case "HTTPRequestBody"))"),
            *Roo::Value::string("HttpRequestBody"));
}

TEST_F(StringCaseFunction, camel_case_converts_common_word_separators)
{
  EXPECT_EQ(*runtime.eval(R"((camel-case "player ship state"))"),
            *Roo::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "player-ship-state"))"),
            *Roo::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "player_ship_state"))"),
            *Roo::Value::string("playerShipState"));
}

TEST_F(StringCaseFunction, camel_case_splits_camel_and_pascal_case)
{
  EXPECT_EQ(*runtime.eval(R"((camel-case "playerShipState"))"),
            *Roo::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "PlayerShipState"))"),
            *Roo::Value::string("playerShipState"));
  EXPECT_EQ(*runtime.eval(R"((camel-case "HTTPRequestBody"))"),
            *Roo::Value::string("httpRequestBody"));
}

TEST_F(StringCaseFunction, string_case_functions_handle_empty_and_separator_only_strings)
{
  EXPECT_EQ(*runtime.eval(R"((kebab-case ""))"), *Roo::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((pascal-case " - _ / "))"), *Roo::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((camel-case " - _ / "))"), *Roo::Value::string(""));
}

TEST_F(StringCaseFunction, string_case_functions_require_string_argument)
{
  EXPECT_THROW(runtime.eval("(kebab-case :not-a-string)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(pascal-case :not-a-string)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(camel-case :not-a-string)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(capitalize :not-a-string)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(capitalize-first :not-a-string)"), Roo::InvocationException);
}

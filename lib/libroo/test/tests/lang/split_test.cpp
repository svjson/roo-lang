
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SplitFunction = RooTest::RuntimeTestFixture;

TEST_F(SplitFunction, splits_basic_string)
{
  // When
  auto result = runtime.eval(R"((split "a,b,c" ","))");

  // Then
  ASSERT_TRUE(Roo::Type::VECTOR.is_type_of(*result));
  ASSERT_EQ(Roo::count(*result), 3);
  EXPECT_EQ(result->elements().at(0)->str(), "a");
  EXPECT_EQ(result->elements().at(1)->str(), "b");
  EXPECT_EQ(result->elements().at(2)->str(), "c");
}

TEST_F(SplitFunction, preserves_empty_strings_between_adjacent_delimiters)
{
  // When
  auto result = runtime.eval(R"((split "a,,b" ","))");

  // Then
  ASSERT_EQ(Roo::count(*result), 3);
  EXPECT_EQ(result->elements().at(0)->str(), "a");
  EXPECT_EQ(result->elements().at(1)->str(), "");
  EXPECT_EQ(result->elements().at(2)->str(), "b");
}

TEST_F(SplitFunction, preserves_empty_string_at_start)
{
  // When
  auto result = runtime.eval(R"((split ",a" ","))");

  // Then
  ASSERT_EQ(Roo::count(*result), 2);
  EXPECT_EQ(result->elements().at(0)->str(), "");
  EXPECT_EQ(result->elements().at(1)->str(), "a");
}

TEST_F(SplitFunction, preserves_empty_string_at_end)
{
  // When
  auto result = runtime.eval(R"((split "a," ","))");

  // Then
  ASSERT_EQ(Roo::count(*result), 2);
  EXPECT_EQ(result->elements().at(0)->str(), "a");
  EXPECT_EQ(result->elements().at(1)->str(), "");
}

TEST_F(SplitFunction, returns_single_element_when_no_match)
{
  // When
  auto result = runtime.eval(R"((split "no-match" ","))");

  // Then
  ASSERT_EQ(Roo::count(*result), 1);
  EXPECT_EQ(result->elements().at(0)->str(), "no-match");
}

TEST_F(SplitFunction, empty_string_input_returns_single_empty_element)
{
  // When
  auto result = runtime.eval(R"((split "" ","))");

  // Then
  ASSERT_EQ(Roo::count(*result), 1);
  EXPECT_EQ(result->elements().at(0)->str(), "");
}

TEST_F(SplitFunction, empty_delimiter_splits_into_characters)
{
  // When
  auto result = runtime.eval(R"((split "abc" ""))");

  // Then
  ASSERT_EQ(Roo::count(*result), 3);
  EXPECT_EQ(result->elements().at(0)->str(), "a");
  EXPECT_EQ(result->elements().at(1)->str(), "b");
  EXPECT_EQ(result->elements().at(2)->str(), "c");
}

TEST_F(SplitFunction, empty_string_with_empty_delimiter_returns_empty_vector)
{
  // When
  auto result = runtime.eval(R"((split "" ""))");

  // Then
  ASSERT_TRUE(Roo::Type::VECTOR.is_type_of(*result));
  ASSERT_EQ(Roo::count(*result), 0);
}

TEST_F(SplitFunction, multi_char_delimiter)
{
  // When
  auto result = runtime.eval(R"((split "a::b::c" "::"))");

  // Then
  ASSERT_EQ(Roo::count(*result), 3);
  EXPECT_EQ(result->elements().at(0)->str(), "a");
  EXPECT_EQ(result->elements().at(1)->str(), "b");
  EXPECT_EQ(result->elements().at(2)->str(), "c");
}

TEST_F(SplitFunction, multi_char_delimiter_does_not_split_partial_match)
{
  // When  -- "::" delimiter does not match a lone ":"
  auto result = runtime.eval(R"((split "a:b::c" "::"))");

  // Then
  ASSERT_EQ(Roo::count(*result), 2);
  EXPECT_EQ(result->elements().at(0)->str(), "a:b");
  EXPECT_EQ(result->elements().at(1)->str(), "c");
}

TEST_F(SplitFunction, split_nil_returns_empty_vector)
{
  // When
  auto result = runtime.eval(R"((split nil "\n"))");

  // Then
  EXPECT_EQ(result->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(Roo::count(*result), 0);
}

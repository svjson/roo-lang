#include <lisple/form.h>
#include <lisple/reader.h>

#include <gtest/gtest.h>

using namespace Lisple;

TEST(Reader, parses_symbol_with_colon_in_body)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps("window:focus-within");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), Symbol("window:focus-within"));
}

TEST(Reader, parses_quoted_symbol_with_colon_in_body)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps("'window:focus-within");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), QuotedSymbol("window:focus-within"));
}

TEST(Reader, parses_keyword_with_namespace_and_colon_in_identifier)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(":ui/menu-item:focus");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), Keyword("ui/menu-item:focus"));
}

TEST(Reader, parses_string_with_common_escapes)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(R"("line 1\nline 2\t\"quoted\"\\tail")");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->get_type(), Form::STRING);
  EXPECT_EQ(sexps.at(0)->as<String>().value, "line 1\nline 2\t\"quoted\"\\tail");
}

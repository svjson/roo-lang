#include <roo/form.h>
#include <roo/reader.h>

#include <gtest/gtest.h>

using namespace Roo;
using namespace Roo::AST;

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

TEST(Reader, parses_keyword_with_dot_in_identifier)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(":ui/button.primary");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), Keyword("ui/button.primary"));
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

TEST(Reader, parses_semicolon_char)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps("';'");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->get_type(), Form::CHAR);
  EXPECT_EQ(sexps.at(0)->as<Char>().value, ';');
}

TEST(Reader, parses_escaped_quote_char)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(R"('\'')");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->get_type(), Form::CHAR);
  EXPECT_EQ(sexps.at(0)->as<Char>().value, '\'');
}

TEST(Reader, parses_brace_chars)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps("['{' '}']");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->size(), 2);
  EXPECT_EQ(sexps.at(0)->get_children().at(0)->as<Char>().value, '{');
  EXPECT_EQ(sexps.at(0)->get_children().at(1)->as<Char>().value, '}');
}

TEST(Reader, parses_char_keyed_map_with_nested_map_values)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps("{' ' {:x 1} '{' {:x 2} '}' {:x 3}}");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->get_type(), Form::MAP);
  ASSERT_EQ(sexps.at(0)->get_children().size(), 6);
  EXPECT_EQ(sexps.at(0)->get_children().at(0)->as<Char>().value, ' ');
  EXPECT_EQ(sexps.at(0)->get_children().at(2)->as<Char>().value, '{');
  EXPECT_EQ(sexps.at(0)->get_children().at(4)->as<Char>().value, '}');
}

TEST(Reader, parses_semicolon_string)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(R"(";")");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->get_type(), Form::STRING);
  EXPECT_EQ(sexps.at(0)->as<String>().value, ";");
}

TEST(Reader, parses_string_starting_with_semicolon)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(R"("; message")");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->get_type(), Form::STRING);
  EXPECT_EQ(sexps.at(0)->as<String>().value, "; message");
}

TEST(Reader, parses_string_containing_semicolon_in_sentence)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(R"("This sentence has two clauses; both should parse.")");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->get_type(), Form::STRING);
  EXPECT_EQ(sexps.at(0)->as<String>().value,
            "This sentence has two clauses; both should parse.");
}

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

TEST(Reader, parses_quoted_symbol_starting_with_a_number)
{
  Reader reader;

  auto sexps = reader.read_sexps("'1");

  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), QuotedSymbol("1"));
}

TEST(Reader, keeps_numbers_inside_quoted_lists_as_numbers)
{
  Reader reader;

  auto sexps = reader.read_sexps("'(1 2 3)");

  ASSERT_EQ(sexps.size(), 1);
  ASSERT_EQ(sexps.at(0)->size(), 3);
  EXPECT_EQ(*sexps.at(0)->get_children().at(0), *Number::make("1"));
  EXPECT_EQ(*sexps.at(0)->get_children().at(1), *Number::make("2"));
  EXPECT_EQ(*sexps.at(0)->get_children().at(2), *Number::make("3"));
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

TEST(Reader, tooling_read_preserves_scalar_occurrences_and_sources)
{
  Reader reader;

  auto forms = reader.read_sexps("[:same :same 42 42]", 1, true);

  ASSERT_EQ(forms.size(), 1);
  const auto& children = forms[0]->get_children();
  ASSERT_EQ(children.size(), 4);
  EXPECT_TRUE(forms[0]->get_source().valid());
  EXPECT_NE(children[0].get(), children[1].get());
  EXPECT_NE(children[2].get(), children[3].get());
  EXPECT_TRUE(children[0]->get_source().valid());
  EXPECT_TRUE(children[1]->get_source().valid());
  EXPECT_TRUE(children[2]->get_source().valid());
  EXPECT_TRUE(children[3]->get_source().valid());
  EXPECT_NE(children[0]->get_source().span.start.column,
            children[1]->get_source().span.start.column);
}

TEST(Reader, evaluation_read_pools_source_less_scalars)
{
  Reader reader;
  Pool pool;

  auto forms = reader.read_sexps_for_evaluation("[:same :same 42 42]", pool, 1, true);

  ASSERT_EQ(forms.size(), 1);
  const auto& children = forms[0]->get_children();
  ASSERT_EQ(children.size(), 4);
  EXPECT_TRUE(forms[0]->get_source().valid());
  EXPECT_EQ(children[0].get(), children[1].get());
  EXPECT_EQ(children[2].get(), children[3].get());
  EXPECT_FALSE(children[0]->get_source().valid());
  EXPECT_FALSE(children[1]->get_source().valid());
  EXPECT_FALSE(children[2]->get_source().valid());
  EXPECT_FALSE(children[3]->get_source().valid());
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

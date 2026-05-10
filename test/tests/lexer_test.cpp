
#include <string>

#include <lisple/lexer.h>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

Lisple::Lexer lexer;

typedef Lisple::Symbol sym;
typedef Lisple::Token tkn;

using namespace ::testing;

TEST(Lexer, parse_single_word)
{
  // Given
  std::string input = "macaroni";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::WORD, "macaroni")));
}

TEST(Lexer, parse_single_word_with_gt_and_lt)
{
  // Given
  std::string input = "m<a>c";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::WORD, "m<a>c")));
}

TEST(Lexer, parse_single_word_with_colon_in_body)
{
  // Given
  std::string input = "window:focus-within";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::WORD, "window:focus-within")));
}

TEST(Lexer, parse_single_number_in_hexadecimal_format)
{
  // Given
  std::string input = "0x0f";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::NUMBER, "15")));
}

TEST(Lexer, parse_simple_form)
{
  // Given
  std::string input = "(trait 'UNOBSERVABLE)";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "trait"),
                          sym(tkn::SQUOT, "'"),
                          sym(tkn::WORD, "UNOBSERVABLE"),
                          sym(tkn::RPAREN, ")")));
}

TEST(Lexer, parse_quoted_symbol_with_colon_in_body)
{
  // Given
  std::string input = "'window:focus-within";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::SQUOT, "'"), sym(tkn::WORD, "window:focus-within")));
}

TEST(Lexer, parse_form_with_string)
{
  // Given
  std::string input = "(actor-stereotype \"maggot\")";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "actor-stereotype"),
                          sym(tkn::STRING, "maggot"),
                          sym(tkn::RPAREN, ")")));
}

TEST(Lexer, parse_form_with_map)
{
  // Given
  std::string input =
    "(interaction {:trigger (action-trigger 'SEARCH) :reactions \"bogus\"})";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "interaction"),
                          sym(tkn::LCURLY, "{"),
                          sym(tkn::KEY, "trigger"),
                          sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "action-trigger"),
                          sym(tkn::SQUOT, "'"),
                          sym(tkn::WORD, "SEARCH"),
                          sym(tkn::RPAREN, ")"),
                          sym(tkn::KEY, "reactions"),
                          sym(tkn::STRING, "bogus"),
                          sym(tkn::RCURLY, "}"),
                          sym(tkn::RPAREN, ")")));
}

TEST(Lexer, parse_keyword_with_namespace_and_colon_in_identifier)
{
  // Given
  std::string input = ":ui/menu-item:focus";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::KEY, "ui/menu-item:focus")));
}

TEST(Lexer, parse_map_with_char_key)
{
  // Given
  std::string input = "{'A' \"Alpha\" 'B' \"Beta\"}";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LCURLY, "{"),
                          sym(tkn::CHAR, "A"),
                          sym(tkn::STRING, "Alpha"),
                          sym(tkn::CHAR, "B"),
                          sym(tkn::STRING, "Beta"),
                          sym(tkn::RCURLY, "}")));
}

TEST(Lexer, parse_form_with_lists_array_and_map)
{
  // Given
  std::string input = "(actor-stereotype {:name (name \"Test\") :traits [(marker-trait "
                      "'UNOBSERVABLE)(marker-trait 'NPC)]})";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "actor-stereotype"),
                          sym(tkn::LCURLY, "{"),
                          sym(tkn::KEY, "name"),
                          sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "name"),
                          sym(tkn::STRING, "Test"),
                          sym(tkn::RPAREN, ")"),
                          sym(tkn::KEY, "traits"),
                          sym(tkn::LBRACKET, "["),
                          sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "marker-trait"),
                          sym(tkn::SQUOT, "'"),
                          sym(tkn::WORD, "UNOBSERVABLE"),
                          sym(tkn::RPAREN, ")"),
                          sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "marker-trait"),
                          sym(tkn::SQUOT, "'"),
                          sym(tkn::WORD, "NPC"),
                          sym(tkn::RPAREN, ")"),
                          sym(tkn::RBRACKET, "]"),
                          sym(tkn::RCURLY, "}"),
                          sym(tkn::RPAREN, ")")));
}

TEST(Lexer, parse_bare_string)
{
  // Given
  std::string input = "\"a string\"";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::STRING, "a string")));
}

TEST(Lexer, parse_bare_string_with_escaped_quote)
{
  // Given
  std::string input = R"("the \"REAL\" deal")";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::STRING, "the \"REAL\" deal")));
}

TEST(Lexer, array_of_numbers)
{
  // Given
  std::string input = "[2 1 0 -1 -2 4.5]";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LBRACKET, "["),
                          sym(tkn::NUMBER, "2"),
                          sym(tkn::NUMBER, "1"),
                          sym(tkn::NUMBER, "0"),
                          sym(tkn::NUMBER, "-1"),
                          sym(tkn::NUMBER, "-2"),
                          sym(tkn::NUMBER, "4.5"),
                          sym(tkn::RBRACKET, "]")));
}

TEST(Lexer, array_of_numbers_including_hexadecimal)
{
  // Given
  std::string input = "[2 1 0x0f 2]";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LBRACKET, "["),
                          sym(tkn::NUMBER, "2"),
                          sym(tkn::NUMBER, "1"),
                          sym(tkn::NUMBER, "15"),
                          sym(tkn::NUMBER, "2"),
                          sym(tkn::RBRACKET, "]")));
}

TEST(Lexer, array_of_numbers_with_hexadecimal_at_endl)
{
  // Given
  std::string input = "[2 1 0x0f]";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LBRACKET, "["),
                          sym(tkn::NUMBER, "2"),
                          sym(tkn::NUMBER, "1"),
                          sym(tkn::NUMBER, "15"),
                          sym(tkn::RBRACKET, "]")));
}

TEST(Lexer, negative_number_at_end_of_array)
{
  // Given
  std::string input = "[2 1 0 -1]";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LBRACKET, "["),
                          sym(tkn::NUMBER, "2"),
                          sym(tkn::NUMBER, "1"),
                          sym(tkn::NUMBER, "0"),
                          sym(tkn::NUMBER, "-1"),
                          sym(tkn::RBRACKET, "]")));
}

TEST(Lexer, comment_only)
{
  // Given
  std::string input1 = "; commented line";
  std::string input2 = ";; commented line";
  std::string input3 = ";;; commented line";
  std::string input4 = ";;;; commented line";

  // Then
  EXPECT_EQ(lexer.read_symbols(input1).size(), 0);
  EXPECT_EQ(lexer.read_symbols(input2).size(), 0);
  EXPECT_EQ(lexer.read_symbols(input3).size(), 0);
  EXPECT_EQ(lexer.read_symbols(input4).size(), 0);
}

TEST(Lexer, comment_on_same_line_as_sexp)
{
  // Given
  std::string input =
    ";; I am a comment and thus don't care about (defun add-five [x] (+ x 5))";

  // Then
  EXPECT_EQ(lexer.read_symbols(input).size(), 0);
}

TEST(Lexer, comment_on_line_above)
{
  // Given
  std::string input = ";; I am a comment that ends\n['a' 1 :key]";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LBRACKET, "["),
                          sym(tkn::CHAR, "a"),
                          sym(tkn::NUMBER, "1"),
                          sym(tkn::KEY, "key"),
                          sym(tkn::RBRACKET, "]")));
}

TEST(Lexer, comment_on_end_of_line)
{
  // Given
  std::string input = "[1 2 3 ;; begins the count\n 4 5 6] ;; ends the count";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LBRACKET, "["),
                          sym(tkn::NUMBER, "1"),
                          sym(tkn::NUMBER, "2"),
                          sym(tkn::NUMBER, "3"),
                          sym(tkn::NUMBER, "4"),
                          sym(tkn::NUMBER, "5"),
                          sym(tkn::NUMBER, "6"),
                          sym(tkn::RBRACKET, "]")));
}

TEST(Lexer, discard__list)
{
  // Given
  std::string input = "#_(fncall arg)";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::HASH, "#"),
                          sym(tkn::USCORE, "_"),
                          sym(tkn::LPAREN, "("),
                          sym(tkn::WORD, "fncall"),
                          sym(tkn::WORD, "arg"),
                          sym(tkn::RPAREN, ")")));
}

TEST(Lexer, ampersand_alone_is_a_word)
{
  // Given
  std::string input = "&";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::WORD, "&")));
}

TEST(Lexer, ampersand_prefix_is_a_word)
{
  // Given
  std::string input = "&rest-args";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols, ElementsAre(sym(tkn::WORD, "&rest-args")));
}

TEST(Lexer, arg_vector_with_optional_and_rest_markers)
{
  // Given
  std::string input = "[a & b &rest]";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(symbols,
              ElementsAre(sym(tkn::LBRACKET, "["),
                          sym(tkn::WORD, "a"),
                          sym(tkn::WORD, "&"),
                          sym(tkn::WORD, "b"),
                          sym(tkn::WORD, "&rest"),
                          sym(tkn::RBRACKET, "]")));
}

TEST(Lexer, discard__string)
{
  // Given
  std::string input = "#_\"Cheese\"";

  // When
  auto symbols = lexer.read_symbols(input);

  // Then
  ASSERT_THAT(
    symbols,
    ElementsAre(sym(tkn::HASH, "#"), sym(tkn::USCORE, "_"), sym(tkn::STRING, "Cheese")));
}

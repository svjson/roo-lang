
#include <memory>
#include <string>
#include <vector>

#include <lisple/form.h>
#include <lisple/lexer.h>
#include <lisple/parser.h>
#include <lisple/type.h>

#include <ext/alloc_traits.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace ::testing;
using namespace Lisple;
using namespace Lisple::AST;

TEST(Parser, parse_simple_form)
{
  // Given
  Parser parser;
  std::vector<TokenSymbol> symbols{TokenSymbol(Token::LPAREN, "("),
                                   TokenSymbol(Token::SYMBOL, "trait"),
                                   TokenSymbol(Token::SQUOT, "'"),
                                   TokenSymbol(Token::SYMBOL, "UNOBSERVABLE"),
                                   TokenSymbol(Token::RPAREN, ")")};

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);

  Object& form = *sexps.at(0);
  ASSERT_EQ(form.size(), 2);

  EXPECT_EQ(form.get_children().at(0)->get_type(), Form::SYMBOL);
  EXPECT_EQ(*form.get_children().at(0), Symbol("trait"));
  EXPECT_EQ(form.get_children().at(1)->get_type(), Form::QUOTED_SYMBOL);
  EXPECT_EQ(*form.get_children().at(1), QuotedSymbol("UNOBSERVABLE"));
}

TEST(Parser, parse_map_with_char_key)
{
  // Given
  Parser parser;
  std::vector<TokenSymbol> symbols{TokenSymbol(Token::LCURLY, "{"),
                                   TokenSymbol(Token::CHAR, "A"),
                                   TokenSymbol(Token::STRING, "Alpha"),
                                   TokenSymbol(Token::CHAR, "B"),
                                   TokenSymbol(Token::STRING, "Beta"),
                                   TokenSymbol(Token::RCURLY, "}")};

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);

  Object& form = *sexps.at(0);
  ASSERT_EQ(form.size(), 2);

  ASSERT_EQ(form.get_type(), Form::MAP);
  EXPECT_EQ(*form.get_children().at(0), Char('A'));
  EXPECT_EQ(*form.get_children().at(1), String("Alpha"));
  EXPECT_EQ(*form.get_children().at(2), Char('B'));
  EXPECT_EQ(*form.get_children().at(3), String("Beta"));
}

TEST(Parser, parse_form_with_multiple_paren_types)
{
  // Given
  Parser parser;
  std::vector<TokenSymbol> symbols{TokenSymbol(Token::LPAREN, "("),
                                   TokenSymbol(Token::SYMBOL, "actor-stereotype"),
                                   TokenSymbol(Token::LCURLY, "{"),
                                   TokenSymbol(Token::KEYWORD, "name"),
                                   TokenSymbol(Token::LPAREN, "("),
                                   TokenSymbol(Token::SYMBOL, "name"),
                                   TokenSymbol(Token::STRING, "Test"),
                                   TokenSymbol(Token::RPAREN, ")"),
                                   TokenSymbol(Token::KEYWORD, "traits"),
                                   TokenSymbol(Token::LBRACKET, "["),
                                   TokenSymbol(Token::LPAREN, "("),
                                   TokenSymbol(Token::SYMBOL, "marker-trait"),
                                   TokenSymbol(Token::SQUOT, "'"),
                                   TokenSymbol(Token::SYMBOL, "UNOBSERVABLE"),
                                   TokenSymbol(Token::RPAREN, ")"),
                                   TokenSymbol(Token::LPAREN, "("),
                                   TokenSymbol(Token::SYMBOL, "marker-trait"),
                                   TokenSymbol(Token::SQUOT, "'"),
                                   TokenSymbol(Token::SYMBOL, "NPC"),
                                   TokenSymbol(Token::RPAREN, ")"),
                                   TokenSymbol(Token::RBRACKET, "]"),
                                   TokenSymbol(Token::RCURLY, "}"),
                                   TokenSymbol(Token::RPAREN, ")")};

  // when
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);

  Object& form = *sexps.at(0);
  ASSERT_EQ(form.size(), 2);
}

TEST(Parser, parse_quoted_list)
{
  Parser parser;
  std::vector<TokenSymbol> symbols{TokenSymbol(Token::SQUOT, "'"),
                                   TokenSymbol(Token::LPAREN, "("),
                                   TokenSymbol(Token::SYMBOL, "to-upper"),
                                   TokenSymbol(Token::STRING, "a string"),
                                   TokenSymbol(Token::RPAREN, ")")};

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);

  Object& obj = *sexps.at(0);
  ASSERT_TRUE(Type::LIST.is_type_of(obj));

  List& list = obj.as<List>();
  ASSERT_EQ(list.size(), 2);
  EXPECT_EQ(*list.get_children().at(0), Symbol("to-upper"));
  EXPECT_EQ(*list.get_children().at(1), String("a string"));

  ASSERT_TRUE(list.is_quoted());
}

TEST(Parser, parse_positive_int_numbers)
{
  // Given
  Parser parser;
  std::vector<TokenSymbol> symbols{
    TokenSymbol(Token::LBRACKET, "["),
    TokenSymbol(Token::NUMBER, "0"),
    TokenSymbol(Token::NUMBER, "1"),
    TokenSymbol(Token::NUMBER, "28"),
    TokenSymbol(Token::NUMBER, "5945"),
    TokenSymbol(Token::RBRACKET, "]"),
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::AST::Vector& vector = sexps.front()->as<Lisple::AST::Vector>();
  ASSERT_EQ(vector.get_children().size(), 4);

  ASSERT_EQ(*vector.get_children().at(0), Lisple::AST::Number(0));
  ASSERT_EQ(*vector.get_children().at(1), Lisple::AST::Number(1));
  ASSERT_EQ(*vector.get_children().at(2), Lisple::AST::Number(28));
  ASSERT_EQ(*vector.get_children().at(3), Lisple::AST::Number(5945));
}

TEST(Parser, parse_negative_int_numbers)
{
  // Given
  Parser parser;
  std::vector<TokenSymbol> symbols{TokenSymbol(Token::LBRACKET, "["),
                                   TokenSymbol(Token::NUMBER, "-1"),
                                   TokenSymbol(Token::NUMBER, "-23"),
                                   TokenSymbol(Token::NUMBER, "-853"),
                                   TokenSymbol(Token::NUMBER, "-19345"),
                                   TokenSymbol(Token::RBRACKET, "]")};

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::AST::Vector& vector = sexps.front()->as<Lisple::AST::Vector>();
  ASSERT_EQ(vector.get_children().size(), 4);

  ASSERT_EQ(*vector.get_children().at(0), Lisple::AST::Number(-1));
  ASSERT_EQ(*vector.get_children().at(1), Lisple::AST::Number(-23));
  ASSERT_EQ(*vector.get_children().at(2), Lisple::AST::Number(-853));
  ASSERT_EQ(*vector.get_children().at(3), Lisple::AST::Number(-19345));
}

TEST(Parser, parse_positive_float_numbers)
{
  // Given
  Parser parser;
  std::vector<TokenSymbol> symbols{
    TokenSymbol(Token::LBRACKET, "["),
    TokenSymbol(Token::NUMBER, "1.0"),
    TokenSymbol(Token::NUMBER, "45.0"),
    TokenSymbol(Token::NUMBER, "1.113"),
    TokenSymbol(Token::NUMBER, "1000.984"),
    TokenSymbol(Token::RBRACKET, "]"),
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::AST::Vector& vector = sexps.front()->as<Lisple::AST::Vector>();
  ASSERT_EQ(vector.get_children().size(), 4);

  ASSERT_EQ(*vector.get_children().at(0), Lisple::AST::Number(1.0f));
  ASSERT_EQ(*vector.get_children().at(1), Lisple::AST::Number(45.0f));
  ASSERT_EQ(*vector.get_children().at(2), Lisple::AST::Number(1.113f));
  ASSERT_EQ(*vector.get_children().at(3), Lisple::AST::Number(1000.984f));
}

TEST(Parser, discard__list_form)
{
  // Given
  Parser parser;
  std::vector<TokenSymbol> symbols{TokenSymbol(Token::HASH, "#"),
                                   TokenSymbol(Token::USCORE, "_"),
                                   TokenSymbol(Token::LPAREN, "("),
                                   TokenSymbol(Token::SYMBOL, "+"),
                                   TokenSymbol(Token::NUMBER, "1"),
                                   TokenSymbol(Token::NUMBER, "1"),
                                   TokenSymbol(Token::RPAREN, ")")};

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::AST::Discard& discard = sexps.front()->as<Lisple::AST::Discard>();
  ASSERT_EQ(discard.to_string(), "#_(+ 1 1)");
}

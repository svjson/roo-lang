
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <ext/alloc_traits.h>
#include <memory>
#include <string>
#include <vector>

#include <lisple/form.h>
#include <lisple/lexer.h>
#include <lisple/parser.h>
#include <lisple/type.h>

using namespace ::testing;
using namespace Lisple;


TEST(Parser, parse_simple_form)
{
  // Given
  Parser parser;
  std::vector<Symbol> symbols {
    Symbol(Token::LPAREN, "("),
    Symbol(Token::WORD, "trait"),
    Symbol(Token::SQUOT, "'"),
    Symbol(Token::WORD, "UNOBSERVABLE"),
    Symbol(Token::RPAREN, ")")
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  //Then
  ASSERT_EQ(sexps.size(), 1);

  Object& form = *sexps.at(0);
  ASSERT_EQ(form.size(), 2);

  EXPECT_EQ(form.get_children().at(0)->get_type(), Form::WORD);
  EXPECT_EQ(*form.get_children().at(0), Word("trait"));
  EXPECT_EQ(form.get_children().at(1)->get_type(), Form::SYMBOL);
  EXPECT_EQ(*form.get_children().at(1), QSymbol("UNOBSERVABLE"));
}


TEST(Parser, parse_map_with_char_key)
{
  // Given
  Parser parser;
  std::vector<Symbol> symbols
  {
    Symbol(Token::LCURLY, "{"),
    Symbol(Token::CHAR, "A"),
    Symbol(Token::STRING, "Alpha"),
    Symbol(Token::CHAR, "B"),
    Symbol(Token::STRING, "Beta"),
    Symbol(Token::RCURLY, "}")
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);

  Object& form = *sexps.at(0);
  ASSERT_EQ(form.size(), 4);

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
  std::vector<Symbol> symbols {
    Symbol(Token::LPAREN, "("),
    Symbol(Token::WORD, "actor-stereotype"),
    Symbol(Token::LCURLY, "{"),
    Symbol(Token::KEY, "name"),
    Symbol(Token::LPAREN, "("),
    Symbol(Token::WORD, "name"),
    Symbol(Token::STRING, "Test"),
    Symbol(Token::RPAREN, ")"),
    Symbol(Token::KEY, "traits"),
    Symbol(Token::LBRACKET, "["),
    Symbol(Token::LPAREN, "("),
    Symbol(Token::WORD, "marker-trait"),
    Symbol(Token::SQUOT, "'"),
    Symbol(Token::WORD, "UNOBSERVABLE"),
    Symbol(Token::RPAREN, ")"),
    Symbol(Token::LPAREN, "("),
    Symbol(Token::WORD, "marker-trait"),
    Symbol(Token::SQUOT, "'"),
    Symbol(Token::WORD, "NPC"),
    Symbol(Token::RPAREN, ")"),
    Symbol(Token::RBRACKET, "]"),
    Symbol(Token::RCURLY, "}"),
    Symbol(Token::RPAREN, ")")
  };

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
  std::vector<Symbol> symbols {
    Symbol(Token::SQUOT, "'"),
    Symbol(Token::LPAREN, "("),
    Symbol(Token::WORD, "to-upper"),
    Symbol(Token::STRING, "a string"),
    Symbol(Token::RPAREN, ")")
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);

  Object& obj = *sexps.at(0);
  ASSERT_TRUE(Type::LIST.is_type_of(obj));

  List& list = obj.as<List>();
  ASSERT_EQ(list.size(), 2);
  EXPECT_EQ(*list.get_children().at(0), Word("to-upper"));
  EXPECT_EQ(*list.get_children().at(1), String("a string"));

  ASSERT_TRUE(list.is_quoted());
}

TEST(Parser, parse_positive_int_numbers)
{
  // Given
  Parser parser;
  std::vector<Symbol> symbols{
    Symbol(Token::LBRACKET, "["),
    Symbol(Token::NUMBER, "0"),
    Symbol(Token::NUMBER, "1"),
    Symbol(Token::NUMBER, "28"),
    Symbol(Token::NUMBER, "5945"),
    Symbol(Token::RBRACKET, "]"),
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::Array& array = sexps.front()->as<Lisple::Array>();
  ASSERT_EQ(array.get_children().size(), 4);

  ASSERT_EQ(*array.get_children().at(0), Lisple::Number(0));
  ASSERT_EQ(*array.get_children().at(1), Lisple::Number(1));
  ASSERT_EQ(*array.get_children().at(2), Lisple::Number(28));
  ASSERT_EQ(*array.get_children().at(3), Lisple::Number(5945));
}

TEST(Parser, parse_negative_int_numbers)
{
  // Given
  Parser parser;
  std::vector<Symbol> symbols{
    Symbol(Token::LBRACKET, "["),
    Symbol(Token::NUMBER, "-1"),
    Symbol(Token::NUMBER, "-23"),
    Symbol(Token::NUMBER, "-853"),
    Symbol(Token::NUMBER, "-19345"),
    Symbol(Token::RBRACKET, "]")
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::Array& array = sexps.front()->as<Lisple::Array>();
  ASSERT_EQ(array.get_children().size(), 4);

  ASSERT_EQ(*array.get_children().at(0), Lisple::Number(-1));
  ASSERT_EQ(*array.get_children().at(1), Lisple::Number(-23));
  ASSERT_EQ(*array.get_children().at(2), Lisple::Number(-853));
  ASSERT_EQ(*array.get_children().at(3), Lisple::Number(-19345));
}

TEST(Parser, parse_positive_float_numbers)
{
  // Given
  Parser parser;
  std::vector<Symbol> symbols{
    Symbol(Token::LBRACKET, "["),
    Symbol(Token::NUMBER, "1.0"),
    Symbol(Token::NUMBER, "45.0"),
    Symbol(Token::NUMBER, "1.113"),
    Symbol(Token::NUMBER, "1000.984"),
    Symbol(Token::RBRACKET, "]"),
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::Array& array = sexps.front()->as<Lisple::Array>();
  ASSERT_EQ(array.get_children().size(), 4);

  ASSERT_EQ(*array.get_children().at(0), Lisple::Number(1.0f));
  ASSERT_EQ(*array.get_children().at(1), Lisple::Number(45.0f));
  ASSERT_EQ(*array.get_children().at(2), Lisple::Number(1.113f));
  ASSERT_EQ(*array.get_children().at(3), Lisple::Number(1000.984f));
}

TEST(Parser, discard__list_form)
{
  // Given
  Parser parser;
  std::vector<Symbol> symbols {
    Symbol(Token::HASH, "#"),
    Symbol(Token::USCORE, "_"),
    Symbol(Token::LPAREN, "("),
    Symbol(Token::WORD, "+"),
    Symbol(Token::NUMBER, "1"),
    Symbol(Token::NUMBER, "1"),
    Symbol(Token::RPAREN, ")")
  };

  // When
  auto sexps = parser.parse_sexps(symbols);

  // Then
  ASSERT_EQ(sexps.size(), 1);
  Lisple::Discard& discard = sexps.front()->as<Lisple::Discard>();
  ASSERT_EQ(discard.to_string(), "#_(+ 1 1)");
}

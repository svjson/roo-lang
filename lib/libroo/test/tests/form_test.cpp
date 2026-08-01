
#include <memory>

#include <roo/exception.h>
#include <roo/form.h>
#include <roo/type.h>

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

TEST(nil, equality)
{
  // Given
  Roo::AST::Nil nil1;
  Roo::AST::Nil nil2;
  Roo::AST::Symbol symbol("some-symbol");

  // Then
  EXPECT_EQ(nil1, nil2);
  EXPECT_FALSE(nil1 == symbol);
  EXPECT_NE(symbol, nil1);
  EXPECT_NE(nil1, symbol);
}

TEST(QuotedSymbol, equality)
{
  // Given
  Roo::AST::Symbol symbol1("some-symbol");
  Roo::AST::Symbol symbol2("other-symbol");
  Roo::AST::Symbol symbol3("some-symbol");
  Roo::AST::QuotedSymbol quoted_symbol("some-symbol");

  // Then
  EXPECT_EQ(symbol1, symbol3);
  EXPECT_NE(symbol1, symbol2);
  EXPECT_NE(symbol1, quoted_symbol);
}

TEST(Keyword, equality)
{
  // Given
  Roo::AST::Keyword key1("traits");
  Roo::AST::Keyword key2("traits");
  Roo::AST::Keyword key3("reactions");
  Roo::AST::Symbol symbol("traits");

  // Then
  EXPECT_EQ(key1, key2);
  EXPECT_EQ(key1, key1);
  EXPECT_NE(key1, key3);
  EXPECT_NE(key1, symbol);
}

TEST(List, get_property__single_entry)
{
  // Given
  Roo::AST::Keyword key1("id");
  Roo::sptr_ast_node_v list_elements{std::make_shared<Roo::AST::Keyword>("id"),
                                     std::make_shared<Roo::AST::String>("ferret")};
  Roo::AST::List list(list_elements);

  // When
  Roo::AST::ASTNode& value = list.get_property(key1);

  // Then
  EXPECT_EQ(value.get_type(), Roo::Form::STRING);
  EXPECT_EQ(value, Roo::AST::String("ferret"));
}

TEST(Map, cannot_be_constructed_with_duplicate_keys)
{
  // Given
  std::shared_ptr<Roo::AST::Keyword> key1 = std::make_shared<Roo::AST::Keyword>("id");
  std::shared_ptr<Roo::AST::String> value1 = std::make_shared<Roo::AST::String>("value");
  std::shared_ptr<Roo::AST::String> value2 =
    std::make_shared<Roo::AST::String>("othervalue");
  Roo::sptr_ast_node_v map_elements = {key1, value1, key1, value2};
  Roo::RooException* exception = nullptr;

  // When
  try
  {
    Roo::AST::Map map(map_elements);
  }
  catch (Roo::RooException& e)
  {
    exception = &e;
  }

  ASSERT_TRUE(exception);
}

TEST(Map, cannot_be_constructed_with_duplicate_keys__different_instances_with_same_value)
{
  // Given
  std::shared_ptr<Roo::AST::Keyword> key1 = std::make_shared<Roo::AST::Keyword>("id");
  std::shared_ptr<Roo::AST::Keyword> key2 = std::make_shared<Roo::AST::Keyword>("id");
  std::shared_ptr<Roo::AST::String> value1 = std::make_shared<Roo::AST::String>("value");
  std::shared_ptr<Roo::AST::String> value2 =
    std::make_shared<Roo::AST::String>("othervalue");
  Roo::sptr_ast_node_v map_elements = {key1, value1, key2, value2};
  Roo::RooException* exception = nullptr;

  // When
  try
  {
    Roo::AST::Map map(map_elements);
  }
  catch (Roo::RooException& e)
  {
    exception = &e;
  }

  ASSERT_TRUE(exception);
}

TEST(Map, get_property___from_map_multiple_entries)
{
  // Given
  Roo::AST::Keyword key("somekey");
  Roo::sptr_ast_node_v map_elements{std::make_shared<Roo::AST::Keyword>("akey"),
                                    std::make_shared<Roo::AST::String>("avalue"),
                                    std::make_shared<Roo::AST::Keyword>("somekey"),
                                    std::make_shared<Roo::AST::String>("somevalue")};
  Roo::AST::Map map(map_elements);

  // When
  Roo::AST::ASTNode& value = map.get_property(key);

  // Then
  EXPECT_TRUE(Roo::Type::STRING.is_type_of(value));
  EXPECT_EQ(value, Roo::AST::String("somevalue"));
}

TEST(Map, map_with_numeric_keys)
{
  Roo::AST::Map map = Roo::AST::Map({Roo::AST::Number::make(17),
                                     Roo::AST::Number::make(18),
                                     Roo::AST::Number::make(18),
                                     Roo::AST::Number::make(6)});

  EXPECT_EQ(map.to_string(), "{17 18 18 6}");
  EXPECT_EQ(map.get_property(Roo::AST::Number(17)).to_string(), "18");
  EXPECT_EQ(map.get_property(Roo::AST::Number(18)).to_string(), "6");
  EXPECT_EQ(map.get_property(Roo::AST::Number(17)), Roo::AST::Number(18));
  EXPECT_EQ(map.get_property(Roo::AST::Number(18)), Roo::AST::Number(6));
}

TEST(Map, has_key)
{
  Roo::sptr_ast_node_v map_elements{std::make_shared<Roo::AST::Keyword>("akey"),
                                    std::make_shared<Roo::AST::String>("avalue"),
                                    std::make_shared<Roo::AST::Keyword>("somekey"),
                                    std::make_shared<Roo::AST::String>("somevalue")};
  Roo::AST::Map map(map_elements);

  // Then
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("akey")));
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("somekey")));

  EXPECT_FALSE(map.has_key(Roo::AST::Keyword("some-random-key")));
  EXPECT_FALSE(map.has_key(Roo::AST::String("avalue")));
  EXPECT_FALSE(map.has_key(Roo::AST::String("somevalue")));
}

TEST(Map, remove_key__existing_front_key)
{
  Roo::sptr_ast_node_v map_elements{std::make_shared<Roo::AST::Keyword>("a-key"),
                                    std::make_shared<Roo::AST::String>("a value"),
                                    std::make_shared<Roo::AST::Keyword>("some-key"),
                                    std::make_shared<Roo::AST::String>("some value"),
                                    std::make_shared<Roo::AST::Keyword>("another-key"),
                                    std::make_shared<Roo::AST::String>("another-value")};
  Roo::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Roo::AST::Keyword("a-key"));

  // Then
  EXPECT_EQ(*result, Roo::AST::String("a value"));
  EXPECT_EQ(map.size(), 2);
  EXPECT_FALSE(map.has_key(Roo::AST::Keyword("a-key")));
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("some-key")));
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(), R"({:some-key "some value" :another-key "another-value"})");
}

TEST(Map, remove_key__existing_middle_key)
{
  Roo::sptr_ast_node_v map_elements{std::make_shared<Roo::AST::Keyword>("a-key"),
                                    std::make_shared<Roo::AST::String>("a value"),
                                    std::make_shared<Roo::AST::Keyword>("some-key"),
                                    std::make_shared<Roo::AST::String>("some value"),
                                    std::make_shared<Roo::AST::Keyword>("another-key"),
                                    std::make_shared<Roo::AST::String>("another-value")};
  Roo::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Roo::AST::Keyword("some-key"));

  // Then
  EXPECT_EQ(*result, Roo::AST::String("some value"));
  EXPECT_EQ(map.size(), 2);
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("a-key")));
  EXPECT_FALSE(map.has_key(Roo::AST::Keyword("some-key")));
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(), R"({:a-key "a value" :another-key "another-value"})");
}

TEST(Map, remove_key__existing_back_key)
{
  Roo::sptr_ast_node_v map_elements{std::make_shared<Roo::AST::Keyword>("a-key"),
                                    std::make_shared<Roo::AST::String>("a value"),
                                    std::make_shared<Roo::AST::Keyword>("some-key"),
                                    std::make_shared<Roo::AST::String>("some value"),
                                    std::make_shared<Roo::AST::Keyword>("another-key"),
                                    std::make_shared<Roo::AST::String>("another-value")};
  Roo::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Roo::AST::Keyword("another-key"));

  // Then
  EXPECT_EQ(*result, Roo::AST::String("another-value"));
  EXPECT_EQ(map.size(), 2);
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("a-key")));
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("some-key")));
  EXPECT_FALSE(map.has_key(Roo::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(), R"({:a-key "a value" :some-key "some value"})");
}

TEST(Map, remove_key__non_existing_key)
{
  Roo::sptr_ast_node_v map_elements{std::make_shared<Roo::AST::Keyword>("a-key"),
                                    std::make_shared<Roo::AST::String>("a value"),
                                    std::make_shared<Roo::AST::Keyword>("some-key"),
                                    std::make_shared<Roo::AST::String>("some value"),
                                    std::make_shared<Roo::AST::Keyword>("another-key"),
                                    std::make_shared<Roo::AST::String>("another-value")};
  Roo::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Roo::AST::Keyword("wildcard-key"));

  // Then
  EXPECT_EQ(*result, *Roo::AST::NIL);
  EXPECT_EQ(map.size(), 3);
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("a-key")));
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("some-key")));
  EXPECT_TRUE(map.has_key(Roo::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(),
            R"({:a-key "a value" :some-key "some value" :another-key "another-value"})");
}

TEST(Symbol, equality)
{
  // Given
  Roo::AST::QuotedSymbol sym1("UNOBSERVABLE");
  Roo::AST::QuotedSymbol sym2("UNOBSERVABLE");
  Roo::AST::QuotedSymbol sym3("INTERACTIVE");
  Roo::AST::Keyword key("INTERACTIVE");

  // Then
  EXPECT_EQ(sym1, sym2);
  EXPECT_EQ(sym1, sym1);
  EXPECT_NE(sym1, sym3);
  EXPECT_NE(sym3, key);
}

TEST(String, equality)
{
  // Given
  Roo::AST::String str1("Boar");
  Roo::AST::String str2("Boar");
  Roo::AST::String str3("Cat");
  Roo::AST::Symbol symbol("Cat");

  // Then
  EXPECT_EQ(str1, str2);
  EXPECT_EQ(str1, str1);
  EXPECT_NE(str1, str3);
  EXPECT_NE(str3, symbol);
}

TEST(String, to_string_escapes_edn_string_content)
{
  Roo::AST::String str("quoted \"value\" with \\ slash\nand\ttab");

  EXPECT_EQ(str.to_string(), R"("quoted \"value\" with \\ slash\nand\ttab")");
}

TEST(List, equality)
{
  // Given
  Roo::sptr_ast_node_v list1_elements{
    std::make_shared<Roo::AST::Symbol>("trait"),
    std::make_shared<Roo::AST::QuotedSymbol>("UNOBSERVABLE")};
  Roo::AST::List list1{list1_elements};

  Roo::sptr_ast_node_v list2_elements{
    std::make_shared<Roo::AST::Symbol>("trait"),
    std::make_shared<Roo::AST::QuotedSymbol>("UNOBSERVABLE")};
  Roo::AST::List list2(list2_elements);

  Roo::sptr_ast_node_v list3_elements{Roo::AST::Number::make(10),
                                      Roo::AST::String::make("ten")};
  Roo::AST::List list3(list3_elements);

  Roo::sptr_ast_node_v list4_elements{Roo::AST::Number::make(10),
                                      Roo::AST::String::make("Macaroni")};
  Roo::AST::List list4(list4_elements);

  Roo::sptr_ast_node_v list5_elements{
    Roo::AST::Number::make(10),
    Roo::AST::String::make("Macaroni"),
    Roo::AST::Symbol::make("trait"),
    std::make_shared<Roo::AST::QuotedSymbol>("UNOBSERVABLE")};
  Roo::AST::List list5(list5_elements);

  // Then
  EXPECT_EQ(list1, list2);
  EXPECT_EQ(list2, list1);

  EXPECT_NE(list1, list3);
  EXPECT_NE(list3, list1);

  EXPECT_NE(list2, list3);
  EXPECT_NE(list3, list2);

  EXPECT_EQ(list3, list3);

  EXPECT_NE(list3, list4);
  EXPECT_NE(list4, list3);

  EXPECT_NE(list5, list4);
  EXPECT_NE(list4, list5);
}

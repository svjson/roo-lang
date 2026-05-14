
#include <memory>

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/type.h>

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

TEST(nil, equality)
{
  // Given
  Lisple::AST::Nil nil1;
  Lisple::AST::Nil nil2;
  Lisple::AST::Symbol symbol("some-symbol");

  // Then
  EXPECT_EQ(nil1, nil2);
  EXPECT_FALSE(nil1 == symbol);
  EXPECT_NE(symbol, nil1);
  EXPECT_NE(nil1, symbol);
}

TEST(QuotedSymbol, equality)
{
  // Given
  Lisple::AST::Symbol symbol1("some-symbol");
  Lisple::AST::Symbol symbol2("other-symbol");
  Lisple::AST::Symbol symbol3("some-symbol");
  Lisple::AST::QuotedSymbol quoted_symbol("some-symbol");

  // Then
  EXPECT_EQ(symbol1, symbol3);
  EXPECT_NE(symbol1, symbol2);
  EXPECT_NE(symbol1, quoted_symbol);
}

TEST(Keyword, equality)
{
  // Given
  Lisple::AST::Keyword key1("traits");
  Lisple::AST::Keyword key2("traits");
  Lisple::AST::Keyword key3("reactions");
  Lisple::AST::Symbol symbol("traits");

  // Then
  EXPECT_EQ(key1, key2);
  EXPECT_EQ(key1, key1);
  EXPECT_NE(key1, key3);
  EXPECT_NE(key1, symbol);
}

TEST(List, get_property__single_entry)
{
  // Given
  Lisple::AST::Keyword key1("id");
  Lisple::sptr_sobject_v list_elements{std::make_shared<Lisple::AST::Keyword>("id"),
                                       std::make_shared<Lisple::AST::String>("ferret")};
  Lisple::AST::List list(list_elements);

  // When
  Lisple::AST::ASTNode& value = list.get_property(key1);

  // Then
  EXPECT_EQ(value.get_type(), Lisple::Form::STRING);
  EXPECT_EQ(value, Lisple::AST::String("ferret"));
}

TEST(Map, cannot_be_constructed_with_duplicate_keys)
{
  // Given
  std::shared_ptr<Lisple::AST::Keyword> key1 = std::make_shared<Lisple::AST::Keyword>("id");
  std::shared_ptr<Lisple::AST::String> value1 =
    std::make_shared<Lisple::AST::String>("value");
  std::shared_ptr<Lisple::AST::String> value2 =
    std::make_shared<Lisple::AST::String>("othervalue");
  Lisple::sptr_sobject_v map_elements = {key1, value1, key1, value2};
  Lisple::LispleException* exception = nullptr;

  // When
  try
  {
    Lisple::AST::Map map(map_elements);
  }
  catch (Lisple::LispleException& e)
  {
    exception = &e;
  }

  ASSERT_TRUE(exception);
}

TEST(Map, cannot_be_constructed_with_duplicate_keys__different_instances_with_same_value)
{
  // Given
  std::shared_ptr<Lisple::AST::Keyword> key1 = std::make_shared<Lisple::AST::Keyword>("id");
  std::shared_ptr<Lisple::AST::Keyword> key2 = std::make_shared<Lisple::AST::Keyword>("id");
  std::shared_ptr<Lisple::AST::String> value1 =
    std::make_shared<Lisple::AST::String>("value");
  std::shared_ptr<Lisple::AST::String> value2 =
    std::make_shared<Lisple::AST::String>("othervalue");
  Lisple::sptr_sobject_v map_elements = {key1, value1, key2, value2};
  Lisple::LispleException* exception = nullptr;

  // When
  try
  {
    Lisple::AST::Map map(map_elements);
  }
  catch (Lisple::LispleException& e)
  {
    exception = &e;
  }

  ASSERT_TRUE(exception);
}

TEST(Map, get_property___from_map_multiple_entries)
{
  // Given
  Lisple::AST::Keyword key("somekey");
  Lisple::sptr_sobject_v map_elements{std::make_shared<Lisple::AST::Keyword>("akey"),
                                      std::make_shared<Lisple::AST::String>("avalue"),
                                      std::make_shared<Lisple::AST::Keyword>("somekey"),
                                      std::make_shared<Lisple::AST::String>("somevalue")};
  Lisple::AST::Map map(map_elements);

  // When
  Lisple::AST::ASTNode& value = map.get_property(key);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(value));
  EXPECT_EQ(value, Lisple::AST::String("somevalue"));
}

TEST(Map, map_with_numeric_keys)
{
  Lisple::AST::Map map = Lisple::AST::Map({Lisple::AST::Number::make(17),
                                           Lisple::AST::Number::make(18),
                                           Lisple::AST::Number::make(18),
                                           Lisple::AST::Number::make(6)});

  EXPECT_EQ(map.to_string(), "{17 18 18 6}");
  EXPECT_EQ(map.get_property(Lisple::AST::Number(17)).to_string(), "18");
  EXPECT_EQ(map.get_property(Lisple::AST::Number(18)).to_string(), "6");
  EXPECT_EQ(map.get_property(Lisple::AST::Number(17)), Lisple::AST::Number(18));
  EXPECT_EQ(map.get_property(Lisple::AST::Number(18)), Lisple::AST::Number(6));
}

TEST(Map, has_key)
{
  Lisple::sptr_sobject_v map_elements{std::make_shared<Lisple::AST::Keyword>("akey"),
                                      std::make_shared<Lisple::AST::String>("avalue"),
                                      std::make_shared<Lisple::AST::Keyword>("somekey"),
                                      std::make_shared<Lisple::AST::String>("somevalue")};
  Lisple::AST::Map map(map_elements);

  // Then
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("akey")));
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("somekey")));

  EXPECT_FALSE(map.has_key(Lisple::AST::Keyword("some-random-key")));
  EXPECT_FALSE(map.has_key(Lisple::AST::String("avalue")));
  EXPECT_FALSE(map.has_key(Lisple::AST::String("somevalue")));
}

TEST(Map, remove_key__existing_front_key)
{
  Lisple::sptr_sobject_v map_elements{
    std::make_shared<Lisple::AST::Keyword>("a-key"),
    std::make_shared<Lisple::AST::String>("a value"),
    std::make_shared<Lisple::AST::Keyword>("some-key"),
    std::make_shared<Lisple::AST::String>("some value"),
    std::make_shared<Lisple::AST::Keyword>("another-key"),
    std::make_shared<Lisple::AST::String>("another-value")};
  Lisple::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Lisple::AST::Keyword("a-key"));

  // Then
  EXPECT_EQ(*result, Lisple::AST::String("a value"));
  EXPECT_EQ(map.size(), 2);
  EXPECT_FALSE(map.has_key(Lisple::AST::Keyword("a-key")));
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("some-key")));
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(), R"({:some-key "some value" :another-key "another-value"})");
}

TEST(Map, remove_key__existing_middle_key)
{
  Lisple::sptr_sobject_v map_elements{
    std::make_shared<Lisple::AST::Keyword>("a-key"),
    std::make_shared<Lisple::AST::String>("a value"),
    std::make_shared<Lisple::AST::Keyword>("some-key"),
    std::make_shared<Lisple::AST::String>("some value"),
    std::make_shared<Lisple::AST::Keyword>("another-key"),
    std::make_shared<Lisple::AST::String>("another-value")};
  Lisple::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Lisple::AST::Keyword("some-key"));

  // Then
  EXPECT_EQ(*result, Lisple::AST::String("some value"));
  EXPECT_EQ(map.size(), 2);
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("a-key")));
  EXPECT_FALSE(map.has_key(Lisple::AST::Keyword("some-key")));
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(), R"({:a-key "a value" :another-key "another-value"})");
}

TEST(Map, remove_key__existing_back_key)
{
  Lisple::sptr_sobject_v map_elements{
    std::make_shared<Lisple::AST::Keyword>("a-key"),
    std::make_shared<Lisple::AST::String>("a value"),
    std::make_shared<Lisple::AST::Keyword>("some-key"),
    std::make_shared<Lisple::AST::String>("some value"),
    std::make_shared<Lisple::AST::Keyword>("another-key"),
    std::make_shared<Lisple::AST::String>("another-value")};
  Lisple::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Lisple::AST::Keyword("another-key"));

  // Then
  EXPECT_EQ(*result, Lisple::AST::String("another-value"));
  EXPECT_EQ(map.size(), 2);
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("a-key")));
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("some-key")));
  EXPECT_FALSE(map.has_key(Lisple::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(), R"({:a-key "a value" :some-key "some value"})");
}

TEST(Map, remove_key__non_existing_key)
{
  Lisple::sptr_sobject_v map_elements{
    std::make_shared<Lisple::AST::Keyword>("a-key"),
    std::make_shared<Lisple::AST::String>("a value"),
    std::make_shared<Lisple::AST::Keyword>("some-key"),
    std::make_shared<Lisple::AST::String>("some value"),
    std::make_shared<Lisple::AST::Keyword>("another-key"),
    std::make_shared<Lisple::AST::String>("another-value")};
  Lisple::AST::Map map(map_elements);

  // When
  auto result = map.remove_key(Lisple::AST::Keyword("wildcard-key"));

  // Then
  EXPECT_EQ(*result, *Lisple::AST::NIL);
  EXPECT_EQ(map.size(), 3);
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("a-key")));
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("some-key")));
  EXPECT_TRUE(map.has_key(Lisple::AST::Keyword("another-key")));
  EXPECT_EQ(map.to_string(),
            R"({:a-key "a value" :some-key "some value" :another-key "another-value"})");
}

TEST(Symbol, equality)
{
  // Given
  Lisple::AST::QuotedSymbol sym1("UNOBSERVABLE");
  Lisple::AST::QuotedSymbol sym2("UNOBSERVABLE");
  Lisple::AST::QuotedSymbol sym3("INTERACTIVE");
  Lisple::AST::Keyword key("INTERACTIVE");

  // Then
  EXPECT_EQ(sym1, sym2);
  EXPECT_EQ(sym1, sym1);
  EXPECT_NE(sym1, sym3);
  EXPECT_NE(sym3, key);
}

TEST(String, equality)
{
  // Given
  Lisple::AST::String str1("Boar");
  Lisple::AST::String str2("Boar");
  Lisple::AST::String str3("Cat");
  Lisple::AST::Symbol symbol("Cat");

  // Then
  EXPECT_EQ(str1, str2);
  EXPECT_EQ(str1, str1);
  EXPECT_NE(str1, str3);
  EXPECT_NE(str3, symbol);
}

TEST(List, equality)
{
  // Given
  Lisple::sptr_sobject_v list1_elements{
    std::make_shared<Lisple::AST::Symbol>("trait"),
    std::make_shared<Lisple::AST::QuotedSymbol>("UNOBSERVABLE")};
  Lisple::AST::List list1{list1_elements};

  Lisple::sptr_sobject_v list2_elements{
    std::make_shared<Lisple::AST::Symbol>("trait"),
    std::make_shared<Lisple::AST::QuotedSymbol>("UNOBSERVABLE")};
  Lisple::AST::List list2(list2_elements);

  Lisple::sptr_sobject_v list3_elements{Lisple::AST::Number::make(10),
                                        Lisple::AST::String::make("ten")};
  Lisple::AST::List list3(list3_elements);

  Lisple::sptr_sobject_v list4_elements{Lisple::AST::Number::make(10),
                                        Lisple::AST::String::make("Macaroni")};
  Lisple::AST::List list4(list4_elements);

  Lisple::sptr_sobject_v list5_elements{
    Lisple::AST::Number::make(10),
    Lisple::AST::String::make("Macaroni"),
    Lisple::AST::Symbol::make("trait"),
    std::make_shared<Lisple::AST::QuotedSymbol>("UNOBSERVABLE")};
  Lisple::AST::List list5(list5_elements);

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

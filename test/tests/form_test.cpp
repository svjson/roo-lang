
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <memory>

#include <lisple/form.h>
#include <lisple/type.h>
#include <lisple/lisple_exception.h>

TEST(nil, equality)
{
  // Given
  Lisple::Nil nil1;
  Lisple::Nil nil2;
  Lisple::Word word("some-word");

  // Then
  EXPECT_EQ(nil1, nil2);
  EXPECT_FALSE(nil1 == word);
  EXPECT_NE(word, nil1);
  EXPECT_NE(nil1, word);
}

TEST(Word, equality)
{
  // Given
  Lisple::Word word1("some-word");
  Lisple::Word word2("other-word");
  Lisple::Word word3("some-word");
  Lisple::QSymbol qsym("some-word");

  // Then
  EXPECT_EQ(word1, word3);
  EXPECT_NE(word1, word2);
  EXPECT_NE(word1, qsym);
}

TEST(Key, equality)
{
  // Given
  Lisple::Key key1("traits");
  Lisple::Key key2("traits");
  Lisple::Key key3("reactions");
  Lisple::Word word("traits");

  // Then
  EXPECT_EQ(key1, key2);
  EXPECT_EQ(key1, key1);
  EXPECT_NE(key1, key3);
  EXPECT_NE(key1, word);
}

TEST(List, get_property__single_entry)
{
  // Given
  Lisple::Key key1("id");
  Lisple::sptr_sobject_v list_elements
  {
    std::make_shared<Lisple::Key>("id"),
    std::make_shared<Lisple::String>("ferret")
  };
  Lisple::List list(list_elements);

  // When
  Lisple::Object& value = list.get_property(key1);

  // Then
  EXPECT_EQ(value.get_type(), Lisple::Form::STRING);
  EXPECT_EQ(value, Lisple::String("ferret"));
}

TEST(Map, cannot_be_constructed_with_duplicate_keys)
{
  // Given
  std::shared_ptr<Lisple::Key> key1 = std::make_shared<Lisple::Key>("id");
  std::shared_ptr<Lisple::String> value1 = std::make_shared<Lisple::String>("value");
  std::shared_ptr<Lisple::String> value2 = std::make_shared<Lisple::String>("othervalue");
  Lisple::sptr_sobject_v map_elements = { key1, value1, key1, value2 };
  Lisple::LispleException* exception = nullptr;

  // When
  try
  {
    Lisple::Map map(map_elements);
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
  std::shared_ptr<Lisple::Key> key1 = std::make_shared<Lisple::Key>("id");
  std::shared_ptr<Lisple::Key> key2 = std::make_shared<Lisple::Key>("id");
  std::shared_ptr<Lisple::String> value1 = std::make_shared<Lisple::String>("value");
  std::shared_ptr<Lisple::String> value2 = std::make_shared<Lisple::String>("othervalue");
  Lisple::sptr_sobject_v map_elements = { key1, value1, key2, value2 };
  Lisple::LispleException* exception = nullptr;

  // When
  try
  {
    Lisple::Map map(map_elements);
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
  Lisple::Key key("somekey");
  Lisple::sptr_sobject_v map_elements
  {
    std::make_shared<Lisple::Key>("akey"),
    std::make_shared<Lisple::String>("avalue"),
    std::make_shared<Lisple::Key>("somekey"),
    std::make_shared<Lisple::String>("somevalue")
  };
  Lisple::Map map(map_elements);

  // When
  Lisple::Object& value = map.get_property(key);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(value));
  EXPECT_EQ(value, Lisple::String("somevalue"));
}


TEST(Symbol, equality)
{
  // Given
  Lisple::QSymbol sym1("UNOBSERVABLE");
  Lisple::QSymbol sym2("UNOBSERVABLE");
  Lisple::QSymbol sym3("INTERACTIVE");
  Lisple::Key key("INTERACTIVE");

  // Then
  EXPECT_EQ(sym1, sym2);
  EXPECT_EQ(sym1, sym1);
  EXPECT_NE(sym1, sym3);
  EXPECT_NE(sym3, key);
}

TEST(String, equality)
{
  // Given
  Lisple::String str1("Boar");
  Lisple::String str2("Boar");
  Lisple::String str3("Cat");
  Lisple::Word word("Cat");

  // Then
  EXPECT_EQ(str1, str2);
  EXPECT_EQ(str1, str1);
  EXPECT_NE(str1, str3);
  EXPECT_NE(str3, word);
}

TEST(Number, equality)
{
  // Given
  Lisple::Number num1(25);
  Lisple::Number num2(25);
  Lisple::String str("25");
  Lisple::Number num3(28);

  EXPECT_EQ(num1, num2);
  EXPECT_EQ(num2, num1);
  EXPECT_NE(num1, str);
  EXPECT_NE(num1, num3);
}

TEST(Number, int_value)
{
  Lisple::Number num1(0);
  Lisple::Number num2(25);
  Lisple::Number num3(40);
  Lisple::Number num4(4000);

  EXPECT_EQ(num1.int_value(), 0);
  EXPECT_EQ(num2.int_value(), 25);
  EXPECT_EQ(num3.int_value(), 40);
  EXPECT_EQ(num4.int_value(), 4000);
}

TEST(Number, num_type_on_construction)
{
  ASSERT_TRUE(Lisple::Number(1).is_num_type(Lisple::NumberType::INT));
  ASSERT_TRUE(Lisple::Number(-1).is_num_type(Lisple::NumberType::INT));
  ASSERT_TRUE(Lisple::Number(45.0f).is_num_type(Lisple::NumberType::FLOAT));
  ASSERT_TRUE(Lisple::Number(1.0f).is_num_type(Lisple::NumberType::FLOAT));
  ASSERT_TRUE(Lisple::Number::make("1")->is_num_type(Lisple::NumberType::INT));
  ASSERT_TRUE(Lisple::Number::make("1.0")->is_num_type(Lisple::NumberType::FLOAT));
}

TEST(List, equality)
{
  // Given
  Lisple::sptr_sobject_v list1_elements
  {
    std::make_shared<Lisple::Word>("trait"),
    std::make_shared<Lisple::QSymbol>("UNOBSERVABLE")
  };
  Lisple::List list1{list1_elements};

  Lisple::sptr_sobject_v list2_elements
  {
    std::make_shared<Lisple::Word>("trait"),
    std::make_shared<Lisple::QSymbol>("UNOBSERVABLE")
  };
  Lisple::List list2(list2_elements);

  // Then
  EXPECT_EQ(list1, list2);
}

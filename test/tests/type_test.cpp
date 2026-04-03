
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/runtime.h>
#include <lisple/type.h>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

TEST(TypeRef, is_type_of)
{
  // Given
  Lisple::List obj;

  // Then
  EXPECT_TRUE(Lisple::Type::LIST.is_type_of(obj));

  EXPECT_FALSE(Lisple::Type::FUNCTION.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::MAP.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::STRING.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::WORD.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::KEY.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::SYMBOL.is_type_of(obj));
}

TEST(TypeRef, rtwrapper_is_type_of)
{
  // Given
  Lisple::sptr_rtval map_val = Lisple::RTValue::map({Lisple::RTValue::keyword("name"),
                                                     Lisple::RTValue::string("Hetty"),
                                                     Lisple::RTValue::keyword("score"),
                                                     Lisple::RTValue::number(5000000)});
  Lisple::RuntimeValueWrapper obj(map_val);

  // Then
  EXPECT_TRUE(Lisple::Type::MAP.is_type_of(obj));

  EXPECT_FALSE(Lisple::Type::FUNCTION.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::STRING.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::LIST.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::WORD.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::KEY.is_type_of(obj));
  EXPECT_FALSE(Lisple::Type::SYMBOL.is_type_of(obj));
}

TEST(MultiRef, is_type_of)
{
  // When
  Lisple::List list;
  Lisple::Array array;
  Lisple::Map map;
  Lisple::String string("");
  Lisple::Boolean boolean(false);

  // Then
  EXPECT_TRUE(Lisple::Type::SEQ.is_type_of(list));
  EXPECT_TRUE(Lisple::Type::SEQ.is_type_of(array));
  EXPECT_TRUE(Lisple::Type::SEQ.is_type_of(map));

  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(string));
  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(boolean));
}

TEST(AnyRef, is_type_of)
{
  // Given
  Lisple::List list;
  Lisple::Map map;
  Lisple::Array array;
  Lisple::String string("string");
  Lisple::Key key("string");
  Lisple::QSymbol symbol("symbol");
  Lisple::Word word("word");

  // Then
  EXPECT_TRUE(Lisple::Type::ANY.is_type_of(list));
  EXPECT_TRUE(Lisple::Type::ANY.is_type_of(map));
  EXPECT_TRUE(Lisple::Type::ANY.is_type_of(array));
  EXPECT_TRUE(Lisple::Type::ANY.is_type_of(string));
  EXPECT_TRUE(Lisple::Type::ANY.is_type_of(key));
  EXPECT_TRUE(Lisple::Type::ANY.is_type_of(symbol));
  EXPECT_TRUE(Lisple::Type::ANY.is_type_of(word));
}

TEST(SeqRef, Array_of_String__is_type_of)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  auto array_of_string = fixture.runtime.eval("[\"string1\" \"string2\" \"string3\"]");
  auto array_of_mixed = fixture.runtime.eval("[\"string1\" :key1 'sym1]");
  Lisple::String string("string");
  Lisple::Key key("string");
  Lisple::QSymbol symbol("symbol");
  Lisple::Word word("word");
  auto list_of_string =
    fixture.runtime.eval(fixture.ctx, "'(\"stringA\" \"stringB\" \"stringC\")");

  // Then
  EXPECT_TRUE(Lisple::Type::ARRAY_OF_STRING.is_type_of(*array_of_string));

  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(*list_of_string));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(*array_of_mixed));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(string));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(key));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(symbol));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(word));
}

TEST(SeqRef, Array_of_Char__is_type_of__nil_is_valid)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  auto array_of_char = fixture.runtime.eval("['A' 'B' 'C' 'D' 'E']");
  auto array_of_char_with_nils = fixture.runtime.eval("['A' 'B' 'C' nil 'E']");
  auto array_of_mixed = fixture.runtime.eval("['A' 'B' 3 \"D\" 'E']");

  // Then
  EXPECT_TRUE(Lisple::Type::ARRAY_OF_CHAR.is_type_of(*array_of_char));
  EXPECT_TRUE(Lisple::Type::ARRAY_OF_CHAR.is_type_of(*array_of_char_with_nils));

  EXPECT_FALSE(Lisple::Type::ARRAY_OF_CHAR.is_type_of(*array_of_mixed));
}

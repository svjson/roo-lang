
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/runtime.h>
#include <lisple/type.h>

#include "host/test_adapters/vehicle_host_adapters.h"
#include "host/test_adapters/vehicle_native_adapters.h"
#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>


using TypeRef = LispleTest::RuntimeTestFixture;
using MultiRef = LispleTest::RuntimeTestFixture;
using AnyRef = LispleTest::RuntimeTestFixture;
using SeqRef = LispleTest::RuntimeTestFixture;
TEST_F(TypeRef, is_type_of)
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

TEST_F(TypeRef, rtwrapper_is_type_of)
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

TEST_F(MultiRef, is_type_of)
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

TEST_F(AnyRef, is_type_of)
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

TEST_F(SeqRef, Array_of_String__is_type_of)
{
  // Given
  auto array_of_string = runtime.eval("[\"string1\" \"string2\" \"string3\"]");
  auto array_of_mixed = runtime.eval("[\"string1\" :key1 'sym1]");
  Lisple::String string("string");
  Lisple::Key key("string");
  Lisple::QSymbol symbol("symbol");
  Lisple::Word word("word");
  auto list_of_string =
    runtime.eval(ctx, "'(\"stringA\" \"stringB\" \"stringC\")");

  // Then
  EXPECT_TRUE(Lisple::Type::ARRAY_OF_STRING.is_type_of(*array_of_string));

  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(*list_of_string));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(*array_of_mixed));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(string));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(key));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(symbol));
  EXPECT_FALSE(Lisple::Type::ARRAY_OF_STRING.is_type_of(word));
}

TEST_F(SeqRef, Array_of_Char__is_type_of__nil_is_valid)
{
  // Given
  auto array_of_char = runtime.eval("['A' 'B' 'C' 'D' 'E']");
  auto array_of_char_with_nils = runtime.eval("['A' 'B' 'C' nil 'E']");
  auto array_of_mixed = runtime.eval("['A' 'B' 3 \"D\" 'E']");

  // Then
  EXPECT_TRUE(Lisple::Type::ARRAY_OF_CHAR.is_type_of(*array_of_char));
  EXPECT_TRUE(Lisple::Type::ARRAY_OF_CHAR.is_type_of(*array_of_char_with_nils));

  EXPECT_FALSE(Lisple::Type::ARRAY_OF_CHAR.is_type_of(*array_of_mixed));
}

TEST_F(SeqRef, Array_of_NativeObject__is_type_of)
{
  // Given
  Lisple::sptr_rtval model_vector = Lisple::RTValue::vector(
    {LispleTest::Native::VehicleModelAdapter::make_unique("Vroom Deluxe", 2),
     LispleTest::Native::VehicleModelAdapter::make_unique("Millenium Falcon", 8),
     LispleTest::Native::VehicleModelAdapter::make_unique("Apollo 13", 4)});

  // Then
  EXPECT_TRUE(LispleTest::ARRAY_OF_VEHICLE_MODEL.is_type_of(*model_vector));
}

TEST_F(SeqRef, Array_of_Map__coerce_to_Array_of_NativeObject)
{
  // Given
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  use_runtime_with(std::move(namespaces), nullptr);
  auto& ctx = *configured_context;
  Lisple::sptr_rtval map_vector =
    Lisple::RTValue::vector({Lisple::RTValue::map({Lisple::RTValue::keyword("model-name"),
                                                   Lisple::RTValue::string("Vroom Deluxe"),
                                                   Lisple::RTValue::keyword("seats"),
                                                   Lisple::RTValue::number(2)}),
                             Lisple::RTValue::map({Lisple::RTValue::keyword("model-name"),
                                                   Lisple::RTValue::string("Vroom Deluxe"),
                                                   Lisple::RTValue::keyword("seats"),
                                                   Lisple::RTValue::number(2)})});

  // When
  auto coercion_result = LispleTest::ARRAY_OF_VEHICLE_MODEL.coerce(ctx, map_vector);

  // Then
  ASSERT_TRUE(coercion_result.success);
  ASSERT_TRUE(LispleTest::ARRAY_OF_VEHICLE_MODEL.is_type_of(*coercion_result.result));
}

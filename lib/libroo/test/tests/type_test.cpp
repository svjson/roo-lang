
#include "host/test_adapters/vehicle_native_adapters.h"
#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <roo/context.h>
#include <roo/form.h>
#include <roo/runtime.h>
#include <roo/type.h>

using TypeRef = RooTest::RuntimeTestFixture;
using MultiRef = RooTest::RuntimeTestFixture;
using AnyRef = RooTest::RuntimeTestFixture;
using SeqRef = RooTest::RuntimeTestFixture;
TEST_F(TypeRef, is_type_of)
{
  // Given
  Roo::AST::List obj;

  // Then
  EXPECT_TRUE(Roo::Type::LIST.is_type_of(obj));

  EXPECT_FALSE(Roo::Type::FUNCTION.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::MAP.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::VECTOR.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::STRING.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::SYMBOL.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::KEYWORD.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::QUOTED_SYMBOL.is_type_of(obj));
}

TEST_F(TypeRef, rtwrapper_is_type_of)
{
  // Given
  Roo::sptr_val map_val = Roo::Value::map({Roo::Value::keyword("name"),
                                           Roo::Value::string("Hetty"),
                                           Roo::Value::keyword("score"),
                                           Roo::Value::number(5000000)});
  Roo::AST::RuntimeValueWrapper obj(map_val);

  // Then
  EXPECT_TRUE(Roo::Type::MAP.is_type_of(obj));

  EXPECT_FALSE(Roo::Type::FUNCTION.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::STRING.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::VECTOR.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::LIST.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::SYMBOL.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::KEYWORD.is_type_of(obj));
  EXPECT_FALSE(Roo::Type::QUOTED_SYMBOL.is_type_of(obj));
}

TEST_F(MultiRef, is_type_of)
{
  // When
  Roo::AST::List list;
  Roo::AST::Vector vector;
  Roo::AST::Map map;
  Roo::AST::String string("");
  Roo::AST::Boolean boolean(false);

  // Then
  EXPECT_TRUE(Roo::Type::SEQ.is_type_of(list));
  EXPECT_TRUE(Roo::Type::SEQ.is_type_of(vector));
  EXPECT_TRUE(Roo::Type::SEQ.is_type_of(map));

  EXPECT_FALSE(Roo::Type::SEQ.is_type_of(string));
  EXPECT_FALSE(Roo::Type::SEQ.is_type_of(boolean));
}

TEST_F(AnyRef, is_type_of)
{
  // Given
  Roo::AST::List list;
  Roo::AST::Map map;
  Roo::AST::Vector vector;
  Roo::AST::String string("string");
  Roo::AST::Keyword key("string");
  Roo::AST::QuotedSymbol quoted_symbol("symbol");
  Roo::AST::Symbol symbol("symbol");

  // Then
  EXPECT_TRUE(Roo::Type::ANY.is_type_of(list));
  EXPECT_TRUE(Roo::Type::ANY.is_type_of(map));
  EXPECT_TRUE(Roo::Type::ANY.is_type_of(vector));
  EXPECT_TRUE(Roo::Type::ANY.is_type_of(string));
  EXPECT_TRUE(Roo::Type::ANY.is_type_of(key));
  EXPECT_TRUE(Roo::Type::ANY.is_type_of(quoted_symbol));
  EXPECT_TRUE(Roo::Type::ANY.is_type_of(symbol));
}

TEST_F(SeqRef, Vector_of_String__is_type_of)
{
  // Given
  auto vector_of_string = runtime.eval("[\"string1\" \"string2\" \"string3\"]");
  auto vector_of_mixed = runtime.eval("[\"string1\" :key1 'sym1]");
  Roo::AST::String string("string");
  Roo::AST::Keyword key("string");
  Roo::AST::QuotedSymbol quoted_symbol("symbol");
  Roo::AST::Symbol symbol("symbol");
  auto list_of_string = runtime.eval(ctx, "'(\"stringA\" \"stringB\" \"stringC\")");

  // Then
  EXPECT_TRUE(Roo::Type::VECTOR_OF_STRING.is_type_of(*vector_of_string));

  EXPECT_FALSE(Roo::Type::VECTOR_OF_STRING.is_type_of(*list_of_string));
  EXPECT_FALSE(Roo::Type::VECTOR_OF_STRING.is_type_of(*vector_of_mixed));
  EXPECT_FALSE(Roo::Type::VECTOR_OF_STRING.is_type_of(string));
  EXPECT_FALSE(Roo::Type::VECTOR_OF_STRING.is_type_of(key));
  EXPECT_FALSE(Roo::Type::VECTOR_OF_STRING.is_type_of(quoted_symbol));
  EXPECT_FALSE(Roo::Type::VECTOR_OF_STRING.is_type_of(symbol));
}

TEST_F(SeqRef, Vector_of_Char__is_type_of__nil_is_valid)
{
  // Given
  auto vector_of_char = runtime.eval("['A' 'B' 'C' 'D' 'E']");
  auto vector_of_char_with_nils = runtime.eval("['A' 'B' 'C' nil 'E']");
  auto vector_of_mixed = runtime.eval("['A' 'B' 3 \"D\" 'E']");

  // Then
  EXPECT_TRUE(Roo::Type::VECTOR_OF_CHAR.is_type_of(*vector_of_char));
  EXPECT_TRUE(Roo::Type::VECTOR_OF_CHAR.is_type_of(*vector_of_char_with_nils));

  EXPECT_FALSE(Roo::Type::VECTOR_OF_CHAR.is_type_of(*vector_of_mixed));
}

TEST_F(SeqRef, Vector_of_NativeObject__is_type_of)
{
  // Given
  Roo::sptr_val model_vector = Roo::Value::vector(
    {RooTest::Native::VehicleModelAdapter::make_unique("Vroom Deluxe", 2),
     RooTest::Native::VehicleModelAdapter::make_unique("Millenium Falcon", 8),
     RooTest::Native::VehicleModelAdapter::make_unique("Apollo 13", 4)});

  // Then
  EXPECT_TRUE(RooTest::VECTOR_OF_VEHICLE_MODEL.is_type_of(*model_vector));
}

TEST_F(SeqRef, Vector_of_Map__coerce_to_Vector_of_NativeObject)
{
  // Given
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  use_runtime_with(std::move(namespaces), nullptr);
  auto& ctx = *configured_context;
  Roo::sptr_val map_vector =
    Roo::Value::vector({Roo::Value::map({Roo::Value::keyword("model-name"),
                                         Roo::Value::string("Vroom Deluxe"),
                                         Roo::Value::keyword("seats"),
                                         Roo::Value::number(2)}),
                        Roo::Value::map({Roo::Value::keyword("model-name"),
                                         Roo::Value::string("Vroom Deluxe"),
                                         Roo::Value::keyword("seats"),
                                         Roo::Value::number(2)})});

  // When
  auto coercion_result = RooTest::VECTOR_OF_VEHICLE_MODEL.coerce(ctx, map_vector);

  // Then
  ASSERT_TRUE(coercion_result.success);
  ASSERT_TRUE(RooTest::VECTOR_OF_VEHICLE_MODEL.is_type_of(*coercion_result.result));
}

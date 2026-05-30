
#include <exception>
#include <string>

#include <roo/form.h>
#include <roo/struct.h>
#include <roo/type.h>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using MapStruct = RooTest::RuntimeTestFixture;
bool __mapstruct_validate(Roo::MapStruct& map_struct, Roo::sptr_ast_node& map)
{
  bool caught = false;
  try
  {
    map_struct.validate("my fine structure", *map);
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
    caught = true;
  }

  return caught;
}

Roo::sptr_ast_node eval_object(Roo::Runtime& runtime, const std::string& source)
{
  return Roo::to_AST(*runtime.eval(source));
}

TEST_F(MapStruct, validate_map_with_string_values)
{
  // Given
  Roo::MapStruct map_struct({{":name", Roo::MapEntryReq(&Roo::Type::STRING, false)},
                             {":description", Roo::MapEntryReq(&Roo::Type::STRING, false)}});

  Roo::sptr_ast_node map =
    eval_object(runtime, "{ :name \"mystring\" :description \"contains text\"}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_FALSE(caught);
}

TEST_F(MapStruct, validate_map_with_unknown_key)
{
  // Given
  Roo::MapStruct map_struct({{":name", Roo::MapEntryReq(&Roo::Type::STRING, false)}});

  Roo::sptr_ast_node map =
    eval_object(runtime, "{ :name \"mystring\" :description \"contains text\"}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_TRUE(caught);
}

TEST_F(MapStruct, validate_map_with_wrong_value_type)
{
  // Given
  Roo::MapStruct map_struct({{":name", Roo::MapEntryReq(&Roo::Type::STRING, false)},
                             {":description", Roo::MapEntryReq(&Roo::Type::STRING, false)}});

  Roo::sptr_ast_node map =
    eval_object(runtime, "{:name \"mystring\" :description \"contains text\"}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_FALSE(caught);
}

TEST_F(MapStruct, validate_map_with_vector_of_string_value)
{
  // Given
  Roo::MapStruct map_struct(
    {{":values", Roo::MapEntryReq(&Roo::Type::VECTOR_OF_STRING, false)}});

  Roo::sptr_ast_node map = eval_object(runtime, "{ :values [\"A\" \"B\" \"C\"]}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_FALSE(caught);
}

TEST_F(MapStruct, get_property_from_map)
{
  // Given
  Roo::MapStruct map_struct({{":value", Roo::MapEntryReq(&Roo::Type::STRING, true)}});

  Roo::sptr_ast_node map = eval_object(runtime, "{ :value \"A fine string!\"}");
  Roo::AST::Keyword key = Roo::AST::Keyword("value");

  // When
  Roo::AST::String value = *map_struct.get_value<Roo::AST::String>(*map, key);

  // Then
  EXPECT_EQ(value, Roo::AST::String("A fine string!"));
}

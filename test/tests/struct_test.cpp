
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <lisple/lisp_reader.h>
#include <lisple/struct.h>
#include <lisple/type.h>
#include <lisple/form.h>

#include "lisp_reader_fixture.h"

bool __mapstruct_validate(Lisple::MapStruct& map_struct, Lisple::sptr_sobject& map)
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

TEST(MapStruct, validate_map_with_string_values)
{
  // Given
  Lisple::MapStruct map_struct({{ ":name", Lisple::MapEntryReq(&Lisple::Type::STRING, false) },
                                { ":description", Lisple::MapEntryReq(&Lisple::Type::STRING, false)}});

  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject map = fixture.lisp_reader.eval("{ :name \"mystring\" :description \"contains text\"}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_FALSE(caught);
}

TEST(MapStruct, validate_map_with_unknown_key)
{
  // Given
  Lisple::MapStruct map_struct({{ ":name", Lisple::MapEntryReq(&Lisple::Type::STRING, false)}});

  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject map = fixture.lisp_reader.eval("{ :name \"mystring\" :description \"contains text\"}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_TRUE(caught);
}

TEST(MapStruct, validate_map_with_wrong_value_type)
{
  // Given
  Lisple::MapStruct map_struct({ { ":name", Lisple::MapEntryReq(&Lisple::Type::STRING, false) },
                                 { ":description", Lisple::MapEntryReq(&Lisple::Type::STRING, false) } });

  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject map = fixture.lisp_reader.eval("{ :name \"mystring\" :description \"contains text\"}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_FALSE(caught);
}

TEST(MapStruct, validate_map_with_array_of_string_value)
{
  // Given
  Lisple::MapStruct map_struct({ { ":values", Lisple::MapEntryReq(&Lisple::Type::ARRAY_OF_STRING, false)}});

  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject map = fixture.lisp_reader.eval("{ :values [\"A\" \"B\" \"C\"]}");

  // When
  bool caught = __mapstruct_validate(map_struct, map);

  // Then
  ASSERT_FALSE(caught);
}

TEST(MapStruct, get_property_from_map)
{
  // Given
  Lisple::MapStruct map_struct({ { ":value", Lisple::MapEntryReq(&Lisple::Type::STRING, true)}});

  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject map = fixture.lisp_reader.eval("{ :value \"A fine string!\"}");
  Lisple::Key key = Lisple::Key("value");

  // When
  Lisple::String value = *map_struct.get_value<Lisple::String>(*map, key);

  // Then
  EXPECT_EQ(value, Lisple::String("A fine string!"));
}

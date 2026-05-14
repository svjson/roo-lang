
#include <lisple/adapter.h>
#include "runtime_fixture.h"
#include <lisple/runtime/seq.h>

#include <gtest/gtest.h>


using FilterFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * FilterFunction - (filter [...] predicate)
 * ======================================================================
 */

TEST_F(FilterFunction, filter_array)
{
  // Given
  auto result = runtime.eval("(filter [1 2 3 4 5 6] even?)");

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*result));

  ASSERT_EQ(Lisple::count(*result), 3);

  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 4);
  EXPECT_EQ(result->elements().at(2)->i64(), 6);
}

TEST_F(FilterFunction, filter_vector_int)
{
  // Given
  std::vector<int> int_v{1, 2, 3, 4, 5, 6};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result = runtime.eval("(filter wrapped-vec even?)");

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*result));

  ASSERT_EQ(result->elements().size(), 3);

  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 4);
  EXPECT_EQ(result->elements().at(2)->i64(), 6);
}

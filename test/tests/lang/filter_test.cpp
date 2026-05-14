
#include "runtime_fixture.h"
#include <lisple/runtime/seq.h>

#include <gtest/gtest.h>


using FilterFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * FilterFunction - (filter [...] predicate)
 * ======================================================================
 */

TEST_F(FilterFunction, filter_vector)
{
  // Given
  auto result = runtime.eval("(filter [1 2 3 4 5 6] even?)");

  // Then
  ASSERT_TRUE(Lisple::Type::VECTOR.is_type_of(*result));

  ASSERT_EQ(Lisple::count(*result), 3);

  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 4);
  EXPECT_EQ(result->elements().at(2)->i64(), 6);
}

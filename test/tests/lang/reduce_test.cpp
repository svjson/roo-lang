
#include <lisple/adapter.h>
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using ReduceFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * ReduceFunction - (reduce [...] reducer-fn)
 * ======================================================================
 */

TEST_F(ReduceFunction, reduce_array_of_num)
{
  // Given
  auto retval = runtime.eval("(reduce [128 64 32 16 8 4 2 1] 0 +)");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(255));
}

TEST_F(ReduceFunction, reduce_vector_int)
{
  // Given
  std::vector<int> int_v{128, 64, 32, 16, 8, 4, 2, 1};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(reduce wrapped-vec 0 +)");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(255));
}

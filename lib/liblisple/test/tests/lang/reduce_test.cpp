
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ReduceFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * ReduceFunction - (reduce [...] reducer-fn)
 * ======================================================================
 */

TEST_F(ReduceFunction, reduce_vector_of_num)
{
  // Given
  auto retval = runtime.eval("(reduce [128 64 32 16 8 4 2 1] 0 +)");

  // Then
  EXPECT_EQ(*retval, *Lisple::Value::number(255));
}

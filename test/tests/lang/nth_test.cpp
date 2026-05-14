
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using NthFunction = LispleTest::RuntimeTestFixture;
TEST_F(NthFunction, nth_valid_numbers)
{
  // Given
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 0)")->i64(), 0);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 1)")->i64(), 1);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 2)")->i64(), 2);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 3)")->i64(), 3);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 4)")->i64(), 4);
}

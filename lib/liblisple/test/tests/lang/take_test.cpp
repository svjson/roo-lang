
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using TakeFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * TakeFunction - (take [...])
 * ===================================
 */
TEST_F(TakeFunction, take)
{
  // Then
  EXPECT_EQ(runtime.eval("(take 2 [1 2 3 4 5 6 7 8 9 10])")->to_string(), "[1 2]");
  EXPECT_EQ(runtime.eval("(take 5 [1 2 3])")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(take 10 [])")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(take 0 [1 2 3])")->to_string(), "[]");
}

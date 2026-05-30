#include "runtime_fixture.h"
#include <gtest/gtest.h>

using EvenPFunction = RooTest::RuntimeTestFixture;
TEST_F(EvenPFunction, odd_test)
{
  // Given
  EXPECT_EQ(*runtime.eval("(even? 1)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(even? 2)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(even? 3)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(even? 4)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(even? 5)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(even? 6)"), *Roo::Constant::BOOL_TRUE);
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using OddPFunction = RooTest::RuntimeTestFixture;
TEST_F(OddPFunction, odd_test)
{
  // Given
  EXPECT_EQ(*runtime.eval("(odd? 1)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(odd? 2)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(odd? 3)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(odd? 4)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(odd? 5)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(odd? 6)"), *Roo::Constant::BOOL_FALSE);
}

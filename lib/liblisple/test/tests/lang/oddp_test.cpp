#include "runtime_fixture.h"

#include <gtest/gtest.h>


using OddPFunction = LispleTest::RuntimeTestFixture;
TEST_F(OddPFunction, odd_test)
{
  // Given
  EXPECT_EQ(*runtime.eval("(odd? 1)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(odd? 2)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(odd? 3)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(odd? 4)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(odd? 5)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(odd? 6)"), *Lisple::Constant::BOOL_FALSE);
}

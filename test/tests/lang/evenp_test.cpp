#include "runtime_fixture.h"

#include <gtest/gtest.h>


using EvenPFunction = LispleTest::RuntimeTestFixture;
TEST_F(EvenPFunction, odd_test)
{
  // Given
  EXPECT_EQ(*runtime.eval("(even? 1)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(even? 2)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(even? 3)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(even? 4)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(even? 5)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(even? 6)"), *Lisple::Constant::BOOL_TRUE);
}

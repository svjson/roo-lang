#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NotFunction = RooTest::RuntimeTestFixture;
TEST_F(NotFunction, booleans)
{
  EXPECT_EQ(*runtime.eval("(not true)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not false)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not (odd? 2))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not (odd? 1))"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(NotFunction, values)
{
  // Given
  runtime.eval("(def my-val 15)");
  runtime.eval("(def other-val nil)");

  // Then
  EXPECT_EQ(*runtime.eval("(not my-val)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not other-val)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not 0)"), *Roo::Constant::BOOL_FALSE);
}

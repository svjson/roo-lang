#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CeilFunction = RooTest::RuntimeTestFixture;
TEST_F(CeilFunction, ceil)
{
  // Given
  EXPECT_EQ(*runtime.eval("(ceil 10.2)"), *Roo::Value::number(11));
  EXPECT_EQ(*runtime.eval("(ceil 10.0)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(ceil 5)"), *Roo::Value::number(5));
  EXPECT_EQ(*runtime.eval("(ceil 19.1)"), *Roo::Value::number(20));
}

TEST_F(CeilFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(ceil nil)"), *Roo::Constant::NIL);
}

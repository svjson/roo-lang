#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CeilFunction = LispleTest::RuntimeTestFixture;
TEST_F(CeilFunction, ceil)
{
  // Given
  EXPECT_EQ(*runtime.eval("(ceil 10.2)"), *Lisple::Value::number(11));
  EXPECT_EQ(*runtime.eval("(ceil 10.0)"), *Lisple::Value::number(10));
  EXPECT_EQ(*runtime.eval("(ceil 5)"), *Lisple::Value::number(5));
  EXPECT_EQ(*runtime.eval("(ceil 19.1)"), *Lisple::Value::number(20));
}

TEST_F(CeilFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(ceil nil)"), *Lisple::Constant::NIL);
}

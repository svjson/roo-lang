#include "runtime_fixture.h"
#include <gtest/gtest.h>

using PowFunction = RooTest::RuntimeTestFixture;

TEST_F(PowFunction, raises_base_to_exponent)
{
  EXPECT_EQ(*runtime.eval("(pow 2 3)"), *Roo::Value::number(8.0f));
  EXPECT_EQ(*runtime.eval("(pow 5 0)"), *Roo::Value::number(1.0f));
  EXPECT_EQ(*runtime.eval("(pow 9 0.5)"), *Roo::Value::number(3.0f));
}

TEST_F(PowFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(pow nil 2)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(pow 2 nil)"), *Roo::Constant::NIL);
}

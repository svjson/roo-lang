
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ClampFunction = RooTest::RuntimeTestFixture;

TEST_F(ClampFunction, returns_value_when_within_range)
{
  EXPECT_EQ(*runtime.eval("(clamp 5 0 10)"),   *Roo::Value::number(5));
  EXPECT_EQ(*runtime.eval("(clamp 0 0 10)"),   *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp 10 0 10)"),  *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(clamp 0 -5 5)"),   *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp 0.5 0 1)"),  *Roo::Value::number(0.5f));
}

TEST_F(ClampFunction, clamps_to_low_when_below_range)
{
  EXPECT_EQ(*runtime.eval("(clamp -1 0 10)"),  *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp -99 0 10)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp 1 3 7)"),    *Roo::Value::number(3));
}

TEST_F(ClampFunction, clamps_to_high_when_above_range)
{
  EXPECT_EQ(*runtime.eval("(clamp 11 0 10)"),  *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(clamp 99 0 10)"),  *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(clamp 9 3 7)"),    *Roo::Value::number(7));
}

TEST_F(ClampFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(clamp nil 0 10)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(clamp 5 nil 10)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(clamp 5 0 nil)"),  *Roo::Constant::NIL);
}

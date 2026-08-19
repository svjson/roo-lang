
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ClampBetweenFunction = RooTest::RuntimeTestFixture;

TEST_F(ClampBetweenFunction, returns_value_when_within_range)
{
  EXPECT_EQ(*runtime.eval("(clamp-between 5 0 10)"), *Roo::Value::number(5));
  EXPECT_EQ(*runtime.eval("(clamp-between 0 0 10)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp-between 10 0 10)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(clamp-between 0.5 0 1)"), *Roo::Value::number(0.5f));
}

TEST_F(ClampBetweenFunction, clamps_to_low_when_below_range)
{
  EXPECT_EQ(*runtime.eval("(clamp-between -1 0 10)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp-between -99 0 10)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp-between 1 3 7)"), *Roo::Value::number(3));
}

TEST_F(ClampBetweenFunction, clamps_to_high_when_above_range)
{
  EXPECT_EQ(*runtime.eval("(clamp-between 11 0 10)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(clamp-between 99 0 10)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(clamp-between 9 3 7)"), *Roo::Value::number(7));
}

TEST_F(ClampBetweenFunction, handles_reversed_range)
{
  EXPECT_EQ(*runtime.eval("(clamp-between 5 10 0)"), *Roo::Value::number(5));
  EXPECT_EQ(*runtime.eval("(clamp-between -1 10 0)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(clamp-between 11 10 0)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(clamp-between 0.5 1 0)"), *Roo::Value::number(0.5f));
}

TEST_F(ClampBetweenFunction, nil_bounds_leave_value_unchanged)
{
  EXPECT_EQ(*runtime.eval("(clamp-between nil 0 10)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(clamp-between 12 nil nil)"), *Roo::Value::number(12));
  EXPECT_EQ(*runtime.eval("(clamp-between 12 nil 10)"), *Roo::Value::number(12));
  EXPECT_EQ(*runtime.eval("(clamp-between 12 10 nil)"), *Roo::Value::number(12));
}

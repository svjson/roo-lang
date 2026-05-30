
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MaxFunction = RooTest::RuntimeTestFixture;
TEST_F(MaxFunction, max)
{
  EXPECT_EQ(*runtime.eval("(max 1 1)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(max 0 1)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(max 1 0)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(max 2 1)"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(max 1 2)"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(max 10 1)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 1 10)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 10 0)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 0 10)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 4 10 9 199 200 8 2)"), *Roo::Value::number(200));
}

TEST_F(MaxFunction, skips_nil_arguments)
{
  EXPECT_EQ(*runtime.eval("(max 1 nil 2)"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(max nil 4 10 2 nil 8 200)"), *Roo::Value::number(200));
}

TEST_F(MaxFunction, all_nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(max nil)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(max nil nil)"), *Roo::Constant::NIL);
}


#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MinFunction = RooTest::RuntimeTestFixture;
TEST_F(MinFunction, min)
{
  EXPECT_EQ(*runtime.eval("(min 1 1)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 0 1)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 1 0)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 2 1)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 1 2)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 10 1)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 1 10)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 10 0)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 0 10)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 4 10 2 8 200)"), *Roo::Value::number(2));
}

TEST_F(MinFunction, skips_nil_arguments)
{
  EXPECT_EQ(*runtime.eval("(min 1 nil 2)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min nil 4 10 2 nil 8 200)"), *Roo::Value::number(2));
}

TEST_F(MinFunction, all_nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(min nil)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(min nil nil)"), *Roo::Constant::NIL);
}

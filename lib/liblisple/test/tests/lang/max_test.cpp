
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MaxFunction = LispleTest::RuntimeTestFixture;
TEST_F(MaxFunction, max)
{
  EXPECT_EQ(*runtime.eval("(max 1 1)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(max 0 1)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(max 1 0)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(max 2 1)"), *Lisple::Value::number(2));
  EXPECT_EQ(*runtime.eval("(max 1 2)"), *Lisple::Value::number(2));
  EXPECT_EQ(*runtime.eval("(max 10 1)"), *Lisple::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 1 10)"), *Lisple::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 10 0)"), *Lisple::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 0 10)"), *Lisple::Value::number(10));
  EXPECT_EQ(*runtime.eval("(max 4 10 9 199 200 8 2)"), *Lisple::Value::number(200));
}

TEST_F(MaxFunction, skips_nil_arguments)
{
  EXPECT_EQ(*runtime.eval("(max 1 nil 2)"), *Lisple::Value::number(2));
  EXPECT_EQ(*runtime.eval("(max nil 4 10 2 nil 8 200)"), *Lisple::Value::number(200));
}

TEST_F(MaxFunction, all_nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(max nil)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(max nil nil)"), *Lisple::Constant::NIL);
}

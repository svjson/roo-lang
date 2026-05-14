
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MinFunction = LispleTest::RuntimeTestFixture;
TEST_F(MinFunction, min)
{
  EXPECT_EQ(*runtime.eval("(min 1 1)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 0 1)"), *Lisple::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 1 0)"), *Lisple::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 2 1)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 1 2)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 10 1)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 1 10)"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(min 10 0)"), *Lisple::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 0 10)"), *Lisple::Value::number(0));
  EXPECT_EQ(*runtime.eval("(min 4 10 2 8 200)"), *Lisple::Value::number(2));
}

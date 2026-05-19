#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AbsFunction = LispleTest::RuntimeTestFixture;

TEST_F(AbsFunction, absolute_value)
{
  EXPECT_EQ(*runtime.eval("(abs -10)"), *Lisple::Value::number(10.0f));
  EXPECT_EQ(*runtime.eval("(abs 10)"), *Lisple::Value::number(10.0f));
}

TEST_F(AbsFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(abs nil)"), *Lisple::Constant::NIL);
}

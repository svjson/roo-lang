#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AbsFunction = RooTest::RuntimeTestFixture;

TEST_F(AbsFunction, absolute_value)
{
  EXPECT_EQ(*runtime.eval("(abs -10)"), *Roo::Value::number(10.0f));
  EXPECT_EQ(*runtime.eval("(abs 10)"), *Roo::Value::number(10.0f));
}

TEST_F(AbsFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(abs nil)"), *Roo::Constant::NIL);
}

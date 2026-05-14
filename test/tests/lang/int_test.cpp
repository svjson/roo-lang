
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IntFunction = LispleTest::RuntimeTestFixture;
TEST_F(IntFunction, char_to_int)
{
  // Given
  EXPECT_EQ(*runtime.eval("(int 'c')"), *Lisple::Value::number(99));
  EXPECT_EQ(*runtime.eval("(int ' ')"), *Lisple::Value::number(32));
}

TEST_F(IntFunction, float_to_int)
{
  // Given
  EXPECT_EQ(*runtime.eval("(int 12.0)"), *Lisple::Value::number(12));
  EXPECT_EQ(*runtime.eval("(int 15.4)"), *Lisple::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.5)"), *Lisple::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.9)"), *Lisple::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 16)"), *Lisple::Value::number(16));
}

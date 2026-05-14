#include "runtime_fixture.h"

#include <gtest/gtest.h>


using CeilFunction = LispleTest::RuntimeTestFixture;
TEST_F(CeilFunction, ceil)
{
  // Given
  EXPECT_EQ(*runtime.eval("(ceil 10.2)"), *Lisple::RTValue::number(11));
  EXPECT_EQ(*runtime.eval("(ceil 10.0)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(ceil 5)"), *Lisple::RTValue::number(5));
  EXPECT_EQ(*runtime.eval("(ceil 19.1)"), *Lisple::RTValue::number(20));
}

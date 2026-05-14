#include <gtest/gtest.h>
#include "runtime_fixture.h"


using DecFunction = LispleTest::RuntimeTestFixture;
TEST_F(DecFunction, decrements_integer)
{
  // Given
  auto result = runtime.eval("(dec 2)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(1));
}

TEST_F(DecFunction, decrements_float)
{
  // Given
  auto result = runtime.eval("(dec 2.5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(1.5));
}

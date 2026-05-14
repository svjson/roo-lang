#include <gtest/gtest.h>
#include "runtime_fixture.h"


using IncFunction = LispleTest::RuntimeTestFixture;
TEST_F(IncFunction, increments_integer)
{
  // Given
  auto result = runtime.eval("(inc 2)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(3));
}

TEST_F(IncFunction, increments_float)
{
  // Given
  auto result = runtime.eval("(inc 2.5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(3.5));
}

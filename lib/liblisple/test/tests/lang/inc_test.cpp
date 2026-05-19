#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IncFunction = LispleTest::RuntimeTestFixture;
TEST_F(IncFunction, increments_integer)
{
  // Given
  auto result = runtime.eval("(inc 2)");

  // Then
  EXPECT_EQ(*result, *Lisple::Value::number(3));
}

TEST_F(IncFunction, increments_float)
{
  // Given
  auto result = runtime.eval("(inc 2.5)");

  // Then
  EXPECT_EQ(*result, *Lisple::Value::number(3.5));
}

TEST_F(IncFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(inc nil)"), *Lisple::Constant::NIL);
}

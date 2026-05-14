#include "runtime_fixture.h"

#include <gtest/gtest.h>

using PlusFunction = LispleTest::RuntimeTestFixture;

TEST_F(PlusFunction, simple_addition)
{
  // When
  auto result = runtime.eval("(+ 10 5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(15));
}

TEST_F(PlusFunction, multiple_arg_addition)
{
  // When
  auto result = runtime.eval("(+ 10 5 2 5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(22));
}

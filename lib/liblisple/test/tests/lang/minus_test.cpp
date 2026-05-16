#include <gtest/gtest.h>
#include "runtime_fixture.h"

using MinusFunction = LispleTest::RuntimeTestFixture;

TEST_F(MinusFunction, simple_subtraction)
{
  // When
  auto result = runtime.eval("(- 10 5)");

  // Then
  EXPECT_EQ(result->to_string(), "5");
}

TEST_F(MinusFunction, on_evaluated_numbers)
{
  // When
  auto result = runtime.eval("(- (+ 10 10) (+ 3 5))");

  // Then
  EXPECT_EQ(result->to_string(), "12");
}

TEST_F(MinusFunction, single_positive_flips_sign)
{
  // When
  auto result = runtime.eval("(- 25)");

  // Then
  ASSERT_EQ(result->to_string(), "-25");
}

TEST_F(MinusFunction, single_negative_flips_sign)
{
  // When
  auto result = runtime.eval("(- -25)");

  // Then
  ASSERT_EQ(result->to_string(), "25");
}

#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(MinusFunction, simple_subtraction)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(- 10 5)");

  // Then
  EXPECT_EQ(*result, Lisple::Number(5));
}

TEST(MinusFunction, on_evaluated_numbers)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(- (+ 10 10) (+ 3 5))");

  // Then
  EXPECT_EQ(*result, Lisple::Number(12));
}

TEST(MinusFunction, single_positive_flips_sign)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(- 25)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(-25));
}

TEST(MinusFunction, single_negative_flips_sign)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(- -25)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(25));
}

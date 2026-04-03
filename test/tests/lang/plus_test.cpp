#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(PlusFunction, simple_addition)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(+ 10 5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(15));
}

TEST(PlusFunction, multiple_arg_addition)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(+ 10 5 2 5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(22));
}

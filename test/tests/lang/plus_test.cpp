#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(PlusFunction, simple_addition)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(+ 10 5)");

  // Then
  EXPECT_EQ(*result, Lisple::Number(15));
}

TEST(PlusFunction, multiple_arg_addition)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(+ 10 5 2 5)");

  // Then
  EXPECT_EQ(*result, Lisple::Number(22));
}

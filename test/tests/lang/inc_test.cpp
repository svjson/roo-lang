#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(IncFunction, increments_integer)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(inc 2)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(3));
}

TEST(IncFunction, increments_float)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(inc 2.5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(3.5));
}

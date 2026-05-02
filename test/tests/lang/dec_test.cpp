#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(DecFunction, decrements_integer)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(dec 2)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(1));
}

TEST(DecFunction, decrements_float)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(dec 2.5)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(1.5));
}

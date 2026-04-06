#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(CeilFunction, ceil)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(ceil 10.2)"), *Lisple::RTValue::number(11));
  EXPECT_EQ(*runtime.eval("(ceil 10.0)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(ceil 5)"), *Lisple::RTValue::number(5));
  EXPECT_EQ(*runtime.eval("(ceil 19.1)"), *Lisple::RTValue::number(20));
}


#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(IntFunction, char_to_int)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(int 'c')"), *Lisple::RTValue::number(99));
  EXPECT_EQ(*runtime.eval("(int ' ')"), *Lisple::RTValue::number(32));
}

TEST(IntFunction, float_to_int)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(int 12.0)"), *Lisple::RTValue::number(12));
  EXPECT_EQ(*runtime.eval("(int 15.4)"), *Lisple::RTValue::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.5)"), *Lisple::RTValue::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.9)"), *Lisple::RTValue::number(15));
  EXPECT_EQ(*runtime.eval("(int 16)"), *Lisple::RTValue::number(16));
}

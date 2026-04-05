
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(MaxFunction, max)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(max 1 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(max 0 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(max 1 0)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(max 2 1)"), *Lisple::RTValue::number(2));
  EXPECT_EQ(*runtime.eval("(max 1 2)"), *Lisple::RTValue::number(2));
  EXPECT_EQ(*runtime.eval("(max 10 1)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(max 1 10)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(max 10 0)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(max 0 10)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(max 4 10 9 199 200 8 2)"), *Lisple::RTValue::number(200));
}

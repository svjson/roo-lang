
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(MinFunction, min)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(min 1 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(min 0 1)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*runtime.eval("(min 1 0)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*runtime.eval("(min 2 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(min 1 2)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(min 10 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(min 1 10)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(min 10 0)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*runtime.eval("(min 0 10)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*runtime.eval("(min 4 10 2 8 200)"), *Lisple::RTValue::number(2));
}

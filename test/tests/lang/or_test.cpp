
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(OrForm, logical_or)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 3))"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 3))"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 2))"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 4))"), *Lisple::B_FALSE);
}

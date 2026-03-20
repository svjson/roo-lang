
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(AndForm, logical_and)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(and (odd? 1) (odd? 3))"), *Lisple::B_TRUE);

  EXPECT_EQ(*runtime.eval("(and (odd? 2) (odd? 3))"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(and (odd? 1) (odd? 2))"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(and (odd? 2) (odd? 4))"), *Lisple::B_FALSE);
}

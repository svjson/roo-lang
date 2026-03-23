#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(NotFunction, booleans)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(not true)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(not false)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(not (odd? 2))"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(not (odd? 1))"), *Lisple::B_FALSE);
}

TEST(NotFunction, values)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-val 15)");
  runtime.eval("(def other-val nil)");

  // Then
  EXPECT_EQ(*runtime.eval("(not my-val)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(not other-val)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(not 0)"), *Lisple::B_FALSE);
}

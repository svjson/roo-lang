#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(NotFunction, booleans)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(not true)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not false)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not (odd? 2))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not (odd? 1))"), *Lisple::Constant::BOOL_FALSE);
}

TEST(NotFunction, values)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-val 15)");
  runtime.eval("(def other-val nil)");

  // Then
  EXPECT_EQ(*runtime.eval("(not my-val)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not other-val)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not 0)"), *Lisple::Constant::BOOL_FALSE);
}

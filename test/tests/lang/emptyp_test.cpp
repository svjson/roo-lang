#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(EmptyPredicateFunction, emptyp_seqs)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("'(\"value1\" \"value2\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("[\"value1\" \"value2\"]"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("{:key1 \"value1\" :key2 \"value2\"}"),
            *Lisple::Constant::BOOL_FALSE);

  EXPECT_EQ(*runtime.eval("'()"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("[]"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("{}"), *Lisple::Constant::BOOL_TRUE);
}

TEST(EmptyPredicateFunction, emptyp_strings)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("\"a string\""), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("\"a\""), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("\" \""), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("\"\""), *Lisple::Constant::BOOL_TRUE);
}

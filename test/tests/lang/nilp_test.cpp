#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(NilPredicateFunction, nil)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(nil? nil)"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(nil? [nil])"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? 0)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? \"\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? :nil)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? 'nil)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? false)"), *Lisple::Constant::BOOL_FALSE);
}

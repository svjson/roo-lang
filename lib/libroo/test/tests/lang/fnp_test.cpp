#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FnPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(FnPredicateFunction, function_values_are_functions)
{
  EXPECT_EQ(*runtime.eval("(fn? (fn [x] x))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(fn? +)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(FnPredicateFunction, defun_values_are_functions)
{
  runtime.eval("(defun identity [x] x)");

  EXPECT_EQ(*runtime.eval("(fn? identity)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(FnPredicateFunction, keywords_are_not_functions)
{
  EXPECT_EQ(*runtime.eval("(fn? :name)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(FnPredicateFunction, ordinary_values_are_not_functions)
{
  EXPECT_EQ(*runtime.eval("(fn? nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(fn? 42)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(fn? \"name\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(fn? {:name \"Roo\"})"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(fn? [1 2 3])"), *Roo::Constant::BOOL_FALSE);
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CallablePredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(CallablePredicateFunction, functions_are_callable)
{
  EXPECT_EQ(*runtime.eval("(callable? (fn [x] x))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(callable? +)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(CallablePredicateFunction, keywords_are_callable)
{
  EXPECT_EQ(*runtime.eval("(callable? :name)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(CallablePredicateFunction, ordinary_values_are_not_callable)
{
  EXPECT_EQ(*runtime.eval("(callable? nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(callable? 42)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(callable? \"name\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(callable? {:name \"Roo\"})"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(callable? [1 2 3])"), *Roo::Constant::BOOL_FALSE);
}

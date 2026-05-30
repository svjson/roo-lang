#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NilPredicateFunction = RooTest::RuntimeTestFixture;
TEST_F(NilPredicateFunction, nil)
{
  EXPECT_EQ(*runtime.eval("(nil? nil)"), *Roo::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(nil? [nil])"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? 0)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? \"\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? :nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? 'nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? false)"), *Roo::Constant::BOOL_FALSE);
}

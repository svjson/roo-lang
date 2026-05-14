#include "runtime_fixture.h"

#include <gtest/gtest.h>


using NilPredicateFunction = LispleTest::RuntimeTestFixture;
TEST_F(NilPredicateFunction, nil)
{
  EXPECT_EQ(*runtime.eval("(nil? nil)"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(nil? [nil])"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? 0)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? \"\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? :nil)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? 'nil)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(nil? false)"), *Lisple::Constant::BOOL_FALSE);
}

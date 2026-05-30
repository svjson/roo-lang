
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using OrForm = RooTest::RuntimeTestFixture;
TEST_F(OrForm, logical_or)
{
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 3))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 3))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 2))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 4))"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(OrForm, returns_last_falsy_value_when_no_value_is_truthy)
{
  EXPECT_EQ(*runtime.eval("(or nil nil)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(or nil false)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(or false nil)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(or false false)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(OrForm, returns_first_truthy_value)
{
  EXPECT_EQ(*runtime.eval("(or nil 2)"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(or false \"value\")"), *Roo::Value::string("value"));
}

TEST_F(OrForm, no_args_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(or)"), *Roo::Constant::NIL);
}

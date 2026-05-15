
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using OrForm = LispleTest::RuntimeTestFixture;
TEST_F(OrForm, logical_or)
{
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 3))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 3))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 2))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 4))"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(OrForm, returns_last_falsy_value_when_no_value_is_truthy)
{
  EXPECT_EQ(*runtime.eval("(or nil nil)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(or nil false)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(or false nil)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(or false false)"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(OrForm, returns_first_truthy_value)
{
  EXPECT_EQ(*runtime.eval("(or nil 2)"), *Lisple::Value::number(2));
  EXPECT_EQ(*runtime.eval("(or false \"value\")"), *Lisple::Value::string("value"));
}

TEST_F(OrForm, no_args_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(or)"), *Lisple::Constant::NIL);
}

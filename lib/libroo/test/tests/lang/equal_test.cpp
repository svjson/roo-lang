
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using EqualsFunction = RooTest::RuntimeTestFixture;
TEST_F(EqualsFunction, ints)
{
  EXPECT_EQ(*runtime.eval("(= 1 1)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= 50 (+ 25 25))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= 999 999)"), *Roo::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(= 1 2)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= 50 (+ 25 250))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= 999 -999)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(EqualsFunction, string)
{
  EXPECT_EQ(*runtime.eval("(= \"test\" \"test\")"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= \"a whole sentence\" \"a whole sentence\")"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= \" test\" \" test\")"), *Roo::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(= \"test\" \" test\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= \"a whole sentence\" \"a_whole_sentence\")"),
            *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= \" test\" \" test \")"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(EqualsFunction, mixed_types)
{
  EXPECT_NE(*runtime.eval("(= \"test\" 'test)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_NE(*runtime.eval("(= :test 'test)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_NE(*runtime.eval("(= :test \"test\")"), *Roo::Constant::BOOL_TRUE);

  EXPECT_NE(*runtime.eval("(= [1 2] {1 2})"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= [1 2] [1 2])"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EqualsFunction, function_values_compare_by_identity)
{
  runtime.eval("(def same-fn (fn [] nil))");

  EXPECT_EQ(*runtime.eval("(= same-fn same-fn)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= (fn [] nil) (fn [] nil))"), *Roo::Constant::BOOL_FALSE);
}

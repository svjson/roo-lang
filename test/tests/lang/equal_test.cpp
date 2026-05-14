
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using EqualsFunction = LispleTest::RuntimeTestFixture;
TEST_F(EqualsFunction, ints)
{
  EXPECT_EQ(*runtime.eval("(= 1 1)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= 50 (+ 25 25))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= 999 999)"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(= 1 2)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= 50 (+ 25 250))"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= 999 -999)"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(EqualsFunction, string)
{
  EXPECT_EQ(*runtime.eval("(= \"test\" \"test\")"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= \"a whole sentence\" \"a whole sentence\")"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= \" test\" \" test\")"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(= \"test\" \" test\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= \"a whole sentence\" \"a_whole_sentence\")"),
            *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= \" test\" \" test \")"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(EqualsFunction, mixed_types)
{
  EXPECT_NE(*runtime.eval("(= \"test\" 'test)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_NE(*runtime.eval("(= :test 'test)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_NE(*runtime.eval("(= :test \"test\")"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_NE(*runtime.eval("(= [1 2] {1 2})"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= [1 2] [1 2])"), *Lisple::Constant::BOOL_TRUE);
}

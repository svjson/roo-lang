
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using NotEqualsFunction = LispleTest::RuntimeTestFixture;
TEST_F(NotEqualsFunction, ints)
{
  EXPECT_TRUE(Lisple::is_truthy(*runtime.eval("(not= 1 2)")));
  EXPECT_TRUE(Lisple::is_truthy(*runtime.eval("(not= 50 (+ 25 250))")));
  EXPECT_TRUE(Lisple::is_truthy(*runtime.eval("(not= 999 -999)")));

  EXPECT_FALSE(Lisple::is_truthy(*runtime.eval("(not= 1 1)")));
  EXPECT_FALSE(Lisple::is_truthy(*runtime.eval("(not= 50 (+ 25 25))")));
  EXPECT_FALSE(Lisple::is_truthy(*runtime.eval("(not= 999 999)")));
}

TEST_F(NotEqualsFunction, chars)
{
  EXPECT_EQ(*runtime.eval("(not= 'a' 'b')"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not= '-' ':')"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not= '.' ',')"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(not= 'a' 'a')"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not= '-' '-')"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not= '.' '.')"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(NotEqualsFunction, string)
{
  EXPECT_EQ(*runtime.eval("(not= \"test\" \" test\")"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not= \"a whole sentence\" \"a_whole_sentence\")"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not= \" test\" \" test \")"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(not= \"test\" \"test\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not= \"a whole sentence\" \"a whole sentence\")"),
            *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not= \" test\" \" test\")"), *Lisple::Constant::BOOL_FALSE);
}


#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(EqualsFunction, ints)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(= 1 1)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(= 50 (+ 25 25))"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(= 999 999)"), *Lisple::B_TRUE);

  EXPECT_EQ(*runtime.eval("(= 1 2)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(= 50 (+ 25 250))"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(= 999 -999)"), *Lisple::B_FALSE);
}

TEST(EqualsFunction, string)
{
  Lisple::Runtime runtime;
  EXPECT_EQ(*runtime.eval("(= \"test\" \"test\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(= \"a whole sentence\" \"a whole sentence\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(= \" test\" \" test\")"), *Lisple::B_TRUE);

  EXPECT_EQ(*runtime.eval("(= \"test\" \" test\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(= \"a whole sentence\" \"a_whole_sentence\")"),
            *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(= \" test\" \" test \")"), *Lisple::B_FALSE);
}

TEST(EqualsFunction, mixed_types)
{
  Lisple::Runtime runtime;
  EXPECT_NE(*runtime.eval("(= \"test\" 'test)"), *Lisple::B_TRUE);
  EXPECT_NE(*runtime.eval("(= :test 'test)"), *Lisple::B_TRUE);
  EXPECT_NE(*runtime.eval("(= :test \"test\")"), *Lisple::B_TRUE);

  EXPECT_NE(*runtime.eval("(= [1 2] {1 2})"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(= [1 2] [1 2])"), *Lisple::B_TRUE);
}


#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(RndFunction, max)
{
  Lisple::Runtime runtime;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 5)")->i32();
    ASSERT_TRUE(rndval >= 0 && rndval <= 4);
  }
}

TEST(RndFunction, min_max)
{
  Lisple::Runtime runtime;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 50 55)")->i32();
    ASSERT_TRUE(rndval >= 50 && rndval <= 54);
  }
}

TEST(RndFunction, min_max__0_0)
{
  Lisple::Runtime runtime;
  int rndval = runtime.eval("(rnd 0 0)")->i32();
  ASSERT_EQ(rndval, 0);
}

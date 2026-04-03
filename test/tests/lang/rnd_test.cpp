
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(RndFunction, max)
{
  Lisple::Runtime runtime;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 5)")->i64();
    ASSERT_TRUE(rndval >= 0 && rndval <= 4);
  }
}

TEST(RndFunction, min_max)
{
  Lisple::Runtime runtime;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 50 55)")->i64();
    ASSERT_TRUE(rndval >= 50 && rndval <= 54);
  }
}

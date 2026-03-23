
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(RndFunction, max)
{
  Lisple::Runtime runtime;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 5)")->as<Lisple::Number>().value;
    ASSERT_TRUE(rndval >= 0 && rndval <= 4);
  }
}

TEST(RndFunction, min_max)
{
  Lisple::Runtime runtime;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 50 55)")->as<Lisple::Number>().value;
    ASSERT_TRUE(rndval >= 50 && rndval <= 54);
  }
}

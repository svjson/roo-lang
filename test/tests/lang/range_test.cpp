
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(RangeFunction, rising_numbers)
{
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval("(range 5 10)")->to_string(), "[5 6 7 8 9 10]");
  EXPECT_EQ(runtime.eval("(range -5 2)")->to_string(), "[-5 -4 -3 -2 -1 0 1 2]");
}

TEST(RangeFunction, descending_numbers)
{
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval("(range 10 7)")->to_string(), "[10 9 8 7]");
  EXPECT_EQ(runtime.eval("(range 2 -5)")->to_string(), "[2 1 0 -1 -2 -3 -4 -5]");
}

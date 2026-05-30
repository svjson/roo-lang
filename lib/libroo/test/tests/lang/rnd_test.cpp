
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RndFunction = RooTest::RuntimeTestFixture;
TEST_F(RndFunction, max)
{
  runtime.eval("(random-seed! 1234)");
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 5)")->i32();
    ASSERT_TRUE(rndval >= 0 && rndval <= 4);
  }
}

TEST_F(RndFunction, min_max)
{
  runtime.eval("(random-seed! 1234)");
  for (int i = 0; i < 1000; i++)
  {
    int rndval = runtime.eval("(rnd 50 55)")->i32();
    ASSERT_TRUE(rndval >= 50 && rndval <= 54);
  }
}

TEST_F(RndFunction, min_max__0_0)
{
  int rndval = runtime.eval("(rnd 0 0)")->i32();
  ASSERT_EQ(rndval, 0);
}

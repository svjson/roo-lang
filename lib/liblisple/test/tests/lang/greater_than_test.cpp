
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using GreaterThanFunction = LispleTest::RuntimeTestFixture;
TEST_F(GreaterThanFunction, greater_than__ints)
{
  // Given
  EXPECT_EQ(runtime.eval("(> 1 0)")->to_string(), "true");
  EXPECT_EQ(runtime.eval("(> 3 2)")->to_string(), "true");
  EXPECT_EQ(runtime.eval("(> 100 10)")->to_string(), "true");

  EXPECT_EQ(runtime.eval("(< 1 0)")->to_string(), "false");
  EXPECT_EQ(runtime.eval("(< 3 2)")->to_string(), "false");
  EXPECT_EQ(runtime.eval("(< 100 10)")->to_string(), "false");
}

TEST_F(GreaterThanFunction, greater_than__strings)
{
  // Given
  EXPECT_EQ(runtime.eval(R"((> "semprini" "bouillabaisse"))")->to_string(), "true");

  EXPECT_EQ(runtime.eval(R"((> "bouillabaisse" "semprini"))")->to_string(), "false");
}

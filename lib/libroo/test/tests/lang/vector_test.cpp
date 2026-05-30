
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using VectorFunction = RooTest::RuntimeTestFixture;
TEST_F(VectorFunction, make_vector)
{
  // Given
  EXPECT_EQ(runtime.eval("(vector 1 2 3 4)")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval(R"((vector 1 "2" :foo 'BAR))")->to_string(), "[1 \"2\" :foo BAR]");
  EXPECT_EQ(runtime.eval("(vector :bork)")->to_string(), "[:bork]");
  EXPECT_EQ(runtime.eval("(vector 1 [2 3])")->to_string(), "[1 [2 3]]");
}

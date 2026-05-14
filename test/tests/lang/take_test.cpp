
#include <lisple/adapter.h>
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using TakeFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * TakeFunction - (take [...])
 * ===================================
 */
TEST_F(TakeFunction, take)
{
  // Given
  std::vector<int> int_v{0, 1, 2, 3, 4};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(runtime.eval("(take 2 [1 2 3 4 5 6 7 8 9 10])")->to_string(), "[1 2]");
  EXPECT_EQ(runtime.eval("(take 5 [1 2 3])")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(take 10 [])")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(take 0 [1 2 3])")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(take 3 wrapped-vec)")->to_string(), "[0 1 2]");
}

#include <string>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RandomSeedBangFunction = RooTest::RuntimeTestFixture;

TEST_F(RandomSeedBangFunction, restarts_runtime_random_sequence)
{
  runtime.eval("(random-seed! 1234)");
  std::string first =
    runtime.eval("[(rnd 1000) (rnd 1000) (rand-nth [0 1 2 3 4])]")->to_string();

  runtime.eval("(random-seed! 1234)");
  std::string second =
    runtime.eval("[(rnd 1000) (rnd 1000) (rand-nth [0 1 2 3 4])]")->to_string();

  EXPECT_EQ(first, second);
}

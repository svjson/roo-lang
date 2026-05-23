#include <exception>
#include <string>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using WithRandomSeedForm = LispleTest::RuntimeTestFixture;

TEST_F(WithRandomSeedForm, returns_deterministic_body_result)
{
  std::string first =
    runtime.eval("(with-random-seed 12 [(rnd 100) (rand-nth [:a :b :c])])")->to_string();
  std::string second =
    runtime.eval("(with-random-seed 12 [(rnd 100) (rand-nth [:a :b :c])])")->to_string();

  EXPECT_EQ(first, second);
}

TEST_F(WithRandomSeedForm, restores_previous_runtime_random_state)
{
  runtime.eval("(random-seed! 42)");
  int expected_first = runtime.eval("(rnd 1000)")->i32();
  int expected_after = runtime.eval("(rnd 1000)")->i32();

  runtime.eval("(random-seed! 42)");
  int actual_first = runtime.eval("(rnd 1000)")->i32();
  runtime.eval("(with-random-seed 7 (rnd 1000) (rnd 1000))");
  int actual_after = runtime.eval("(rnd 1000)")->i32();

  EXPECT_EQ(actual_first, expected_first);
  EXPECT_EQ(actual_after, expected_after);
}

TEST_F(WithRandomSeedForm, restores_previous_runtime_random_state_on_error)
{
  runtime.eval("(random-seed! 42)");
  int expected_first = runtime.eval("(rnd 1000)")->i32();
  int expected_after = runtime.eval("(rnd 1000)")->i32();

  runtime.eval("(random-seed! 42)");
  int actual_first = runtime.eval("(rnd 1000)")->i32();
  EXPECT_THROW(runtime.eval(R"((with-random-seed 7 (rnd 1000) (+ "bad" 1)))"),
               std::exception);
  int actual_after = runtime.eval("(rnd 1000)")->i32();

  EXPECT_EQ(actual_first, expected_first);
  EXPECT_EQ(actual_after, expected_after);
}

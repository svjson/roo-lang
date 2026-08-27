#include <chrono>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RooTest::RuntimeTestFixture;

TEST_F(RuntimeTestFixture, sleep_parks_until_the_requested_duration_elapses)
{
  const auto started = std::chrono::steady_clock::now();

  Roo::sptr_val result = runtime.eval("(sleep! 20)");

  const auto elapsed = std::chrono::steady_clock::now() - started;
  EXPECT_EQ(result, Roo::Constant::NIL);
  EXPECT_GE(elapsed, std::chrono::milliseconds(15));
}

TEST_F(RuntimeTestFixture, sleep_accepts_zero_milliseconds)
{
  EXPECT_EQ(runtime.eval("(sleep! 0)"), Roo::Constant::NIL);
}

TEST_F(RuntimeTestFixture, sleep_rejects_invalid_durations)
{
  EXPECT_THROW(runtime.eval("(sleep! -1)"), Roo::RooException);
  EXPECT_THROW(runtime.eval("(sleep! 1.5)"), Roo::RooException);
  EXPECT_THROW(runtime.eval("(sleep! \"1\")"), Roo::RooException);
}

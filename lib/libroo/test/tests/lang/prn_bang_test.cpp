#include "runtime_fixture.h"
#include <gtest/gtest.h>

using PrnBangFunction = RooTest::RuntimeTestFixture;

TEST_F(PrnBangFunction, prints_with_a_trailing_newline)
{
  testing::internal::CaptureStdout();
  runtime.eval(R"((prn! "Hello" "World"))");
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "Hello World\n");
}

TEST_F(PrnBangFunction, returns_nil)
{
  EXPECT_EQ(runtime.eval(R"((prn! "Hello"))")->to_string(), "nil");
}

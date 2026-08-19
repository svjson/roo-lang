#include <iostream>
#include <sstream>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using PrBangFunction = RooTest::RuntimeTestFixture;

TEST_F(PrBangFunction, prints_without_a_trailing_newline)
{
  testing::internal::CaptureStdout();
  runtime.eval(R"((pr! "Hello" "World"))");
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "Hello World");
}

TEST_F(PrBangFunction, returns_nil)
{
  testing::internal::CaptureStdout();
  auto result = runtime.eval(R"((pr! "Hello"))");
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "Hello");
  EXPECT_EQ(result->to_string(), "nil");
}

TEST_F(PrBangFunction, flushes_stdout)
{
  class FlushTrackingBuffer : public std::stringbuf
  {
   public:
    bool flushed = false;

   protected:
    int sync() override
    {
      flushed = true;
      return std::stringbuf::sync();
    }
  } output;

  auto* original = std::cout.rdbuf(&output);
  runtime.eval(R"((pr! "Hello"))");
  std::cout.rdbuf(original);

  EXPECT_TRUE(output.flushed);
  EXPECT_EQ(output.str(), "Hello");
}

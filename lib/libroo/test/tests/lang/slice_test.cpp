#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SliceFunction = RooTest::RuntimeTestFixture;

TEST_F(SliceFunction, extracts_half_open_range)
{
  EXPECT_EQ(runtime.eval("(slice [1 2 3 4 5] 1 4)")->to_string(), "[2 3 4]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3 4 5] 3)")->to_string(), "[4 5]");
}

TEST_F(SliceFunction, resolves_negative_indexes_from_end)
{
  EXPECT_EQ(runtime.eval("(slice [1 2 3 4 5] 1 -1)")->to_string(), "[2 3 4]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] -1)")->to_string(), "[3]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] -20 -1)")->to_string(), "[1 2]");
}

TEST_F(SliceFunction, clamps_bounds_to_sequence_size)
{
  EXPECT_EQ(runtime.eval("(slice [1 2 3] 1 20)")->to_string(), "[2 3]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] 0 2147483648)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] 2147483648)")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] -2147483649)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] -9223372036854775808)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] 20)")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] 2 2)")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(slice [1 2 3] 2 1)")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(slice nil 0 2)")->to_string(), "[]");
}

TEST_F(SliceFunction, accepts_lists)
{
  EXPECT_EQ(runtime.eval("(slice '(1 2 3 4) 1 3)")->to_string(), "[2 3]");
}

TEST_F(SliceFunction, preserves_string_input_type)
{
  EXPECT_EQ(runtime.eval(R"((slice "abcde" 1 4))")->str(), "bcd");
  EXPECT_EQ(runtime.eval(R"((slice "abcde" 3))")->str(), "de");
  EXPECT_EQ(runtime.eval(R"((slice "abcde" 1 -1))")->str(), "bcd");
  EXPECT_EQ(runtime.eval(R"((slice "abc" -1))")->str(), "c");
  EXPECT_EQ(runtime.eval(R"((slice "abc" 1 20))")->str(), "bc");
  EXPECT_EQ(runtime.eval(R"((slice "abc" 2 1))")->str(), "");
  EXPECT_EQ(runtime.eval(R"((slice "" 1))")->str(), "");
}

TEST_F(SliceFunction, rejects_fractional_indexes)
{
  EXPECT_ANY_THROW(runtime.eval("(slice [1 2 3] 0.5)"));
  EXPECT_ANY_THROW(runtime.eval("(slice [1 2 3] 0 1.5)"));
}

TEST_F(SliceFunction, rejects_maps)
{
  EXPECT_THROW(runtime.eval("(slice {:a 1} 0 1)"), Roo::InvocationException);
}

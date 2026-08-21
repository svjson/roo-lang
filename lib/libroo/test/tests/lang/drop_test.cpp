#include <vector>

#include <roo/host/std_adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DropFunction = RooTest::RuntimeTestFixture;

TEST_F(DropFunction, removes_half_open_range)
{
  EXPECT_EQ(runtime.eval("(drop [1 2 3 4 5] 1 4)")->to_string(), "[1 5]");
  EXPECT_EQ(runtime.eval("(drop [1 2 3 4 5] 2)")->to_string(), "[1 2]");
}

TEST_F(DropFunction, resolves_negative_indexes_from_end)
{
  EXPECT_EQ(runtime.eval("(drop [1 2 3 4 5] 1 -1)")->to_string(), "[1 5]");
  EXPECT_EQ(runtime.eval("(drop [1 2 3] -1)")->to_string(), "[1 2]");
  EXPECT_EQ(runtime.eval("(drop [1 2 3] -20 -1)")->to_string(), "[3]");
}

TEST_F(DropFunction, clamps_bounds_and_preserves_reversed_ranges)
{
  EXPECT_EQ(runtime.eval("(drop [1 2 3] 20)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(drop [1 2 3] 2147483648)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(drop [1 2 3] 2 1)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(drop [1 2 3] 2 2)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(drop [] 2)")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(drop nil 2)")->to_string(), "[]");
}

TEST_F(DropFunction, accepts_lists_and_host_sequences)
{
  EXPECT_EQ(runtime.eval("(drop '(1 2 3) 1)")->to_string(), "[1]");

  std::vector<int> values = {1, 2, 3, 4};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));
  EXPECT_EQ(runtime.eval("(drop values 1 3)")->to_string(), "[1 4]");
}

TEST_F(DropFunction, preserves_string_input_type)
{
  EXPECT_EQ(runtime.eval(R"((drop "abcde" 1 4))")->str(), "ae");
  EXPECT_EQ(runtime.eval(R"((drop "abcde" 2))")->str(), "ab");
  EXPECT_EQ(runtime.eval(R"((drop "abcde" 1 -1))")->str(), "ae");
  EXPECT_EQ(runtime.eval(R"((drop "abc" -1))")->str(), "ab");
  EXPECT_EQ(runtime.eval(R"((drop "abc" 2 1))")->str(), "abc");
  EXPECT_EQ(runtime.eval(R"((drop "abc" 20))")->str(), "abc");
  EXPECT_EQ(runtime.eval(R"((drop "" 1))")->str(), "");
}

TEST_F(DropFunction, rejects_fractional_indexes)
{
  EXPECT_ANY_THROW(runtime.eval("(drop [1 2 3] 1.5)"));
  EXPECT_ANY_THROW(runtime.eval("(drop [1 2 3] 0 1.5)"));
}

TEST_F(DropFunction, rejects_maps)
{
  EXPECT_THROW(runtime.eval("(drop {:a 1} 1)"), Roo::InvocationException);
}

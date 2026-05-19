
#include <vector>

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
  // Then
  EXPECT_EQ(runtime.eval("(take 2 [1 2 3 4 5 6 7 8 9 10])")->to_string(), "[1 2]");
  EXPECT_EQ(runtime.eval("(take 5 [1 2 3])")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(take 10 [])")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(take 0 [1 2 3])")->to_string(), "[]");
}

TEST_F(TakeFunction, take_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((take 2 "abc"))")->to_string(), "['a' 'b']");
}

TEST_F(TakeFunction, take_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(take 3 {:a 1 :b 2})")->to_string(), "[:a 1 :b]");
}

TEST_F(TakeFunction, take_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(take 2 values)")->to_string(), "[1 2]");
}

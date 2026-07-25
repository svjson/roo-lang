#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ReverseFunction = RooTest::RuntimeTestFixture;

TEST_F(ReverseFunction, reverse_vector)
{
  EXPECT_EQ(runtime.eval("(reverse [1 2 3])")->to_string(), "[3 2 1]");
  EXPECT_EQ(runtime.eval("(reverse [])")->to_string(), "[]");
}

TEST_F(ReverseFunction, reverse_does_not_mutate_input)
{
  runtime.eval("(def values [1 2 3])");

  EXPECT_EQ(runtime.eval("(reverse values)")->to_string(), "[3 2 1]");
  EXPECT_EQ(runtime.eval("values")->to_string(), "[1 2 3]");
}

TEST_F(ReverseFunction, reverse_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((reverse "abc"))")->to_string(), "['c' 'b' 'a']");
}

TEST_F(ReverseFunction, reverse_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(reverse {:a 1 :b 2})")->to_string(), "[2 :b 1 :a]");
}

TEST_F(ReverseFunction, reverse_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(reverse values)")->to_string(), "[3 2 1]");
}

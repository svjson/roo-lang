#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IndexOfFunction = RooTest::RuntimeTestFixture;

TEST_F(IndexOfFunction, finds_index_in_vector)
{
  EXPECT_EQ(runtime.eval(R"((index-of ["a" "b" "c"] "b"))")->to_string(), "1");
}

TEST_F(IndexOfFunction, returns_nil_when_value_is_missing)
{
  EXPECT_EQ(runtime.eval(R"((index-of ["a" "b" "c"] "z"))")->to_string(), "nil");
}

TEST_F(IndexOfFunction, finds_index_in_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((index-of "abc" 'b'))")->to_string(), "1");
}

TEST_F(IndexOfFunction, finds_index_in_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(index-of {:a 1 :b 2} :b)")->to_string(), "2");
}

TEST_F(IndexOfFunction, finds_index_in_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(index-of values 2)")->to_string(), "1");
}

#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RemoveFunction = LispleTest::RuntimeTestFixture;

TEST_F(RemoveFunction, removes_from_vector)
{
  EXPECT_EQ(runtime.eval("(remove [1 2 3] even?)")->to_string(), "[1 3]");
}

TEST_F(RemoveFunction, removes_from_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((remove "abc" (fn [c] (= c 'b'))))")->to_string(), "['a' 'c']");
}

TEST_F(RemoveFunction, removes_from_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(remove {:a 1 :b 2} keyword?)")->to_string(), "[1 2]");
}

TEST_F(RemoveFunction, removes_from_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(remove values even?)")->to_string(), "[1 3]");
}

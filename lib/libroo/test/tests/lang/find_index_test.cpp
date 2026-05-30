#include <vector>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/adapter.h>

using FindIndexFunction = RooTest::RuntimeTestFixture;

TEST_F(FindIndexFunction, finds_index_in_vector)
{
  EXPECT_EQ(runtime.eval("(find-index [1 2 3] even?)")->to_string(), "1");
}

TEST_F(FindIndexFunction, finds_index_in_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((find-index "abc" (fn [c] (= c 'b'))))")->to_string(), "1");
}

TEST_F(FindIndexFunction, finds_index_in_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(find-index {:a 1 :b 2} (fn [x] (= x :b)))")->to_string(), "2");
}

TEST_F(FindIndexFunction, finds_index_in_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(find-index values even?)")->to_string(), "1");
}

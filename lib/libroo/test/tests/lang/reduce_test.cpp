
#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ReduceFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * ReduceFunction - (reduce [...] reducer-fn)
 * ======================================================================
 */

TEST_F(ReduceFunction, reduce_vector_of_num)
{
  // Given
  auto retval = runtime.eval("(reduce [128 64 32 16 8 4 2 1] 0 +)");

  // Then
  EXPECT_EQ(*retval, *Roo::Value::number(255));
}

TEST_F(ReduceFunction, reduces_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((reduce "abc" "" (fn [acc c] (str acc c))))")->to_string(),
            R"("abc")");
}

TEST_F(ReduceFunction, reduces_map_as_interleaved_sequence)
{
  EXPECT_EQ(
    runtime.eval("(reduce {:a 1 :b 2} [] (fn [acc x] (concat acc [x])))")->to_string(),
    "[:a 1 :b 2]");
}

TEST_F(ReduceFunction, reduces_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(reduce values 0 +)")->to_string(), "6");
}

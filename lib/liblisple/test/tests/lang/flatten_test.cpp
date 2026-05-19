#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FlattenFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * FlattenFunction - (flatten [...] [...])
 * ===================================
 */

TEST_F(FlattenFunction, vector_of_vector)
{
  // Given
  auto retval = runtime.eval("(flatten [[1 2 3] [4 5 6] [7 8 9]])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST_F(FlattenFunction, nested_vectors)
{
  // Given
  auto retval = runtime.eval("(flatten [1 [2 3] [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST_F(FlattenFunction, maps_are_leaf_values)
{
  // Given
  auto retval = runtime.eval("(flatten [[{:x 1 :y 2}] [{:x 3 :y 4}]])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[{:x 1 :y 2} {:x 3 :y 4}]"));
}

TEST_F(FlattenFunction, flattens_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((flatten "ab"))")->to_string(), "['a' 'b']");
}

TEST_F(FlattenFunction, flattens_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(flatten {:a [1 2] :b 3})")->to_string(), "[:a 1 2 :b 3]");
}

TEST_F(FlattenFunction, flattens_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(flatten values)")->to_string(), "[1 2 3]");
}


#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MapFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * MapFunction - (map [...] transform-fn)
 * ======================================================================
 */

TEST_F(MapFunction, map_single_vector_of_numbers)
{
  // Given
  auto retval = runtime.eval("(map [1 2 3] (fn [num] (* num 2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6]");
}

TEST_F(MapFunction, map_two_vectors_of_numbers)
{
  // Given
  auto retval = runtime.eval("(map [1 2 3] [30 20 10] (fn [n1 n2] (+ n1 n2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[31 22 13]");
}

TEST_F(MapFunction, map_two_vecs_of_different_sizes)
{
  // Given
  auto retval = runtime.eval("(map [1 2 3 4] [2 1] (fn [n1 n2] (* n1 n2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 2 nil nil]");
}

TEST_F(MapFunction, map_using_keyword)
{
  // Given
  auto result = runtime.eval("(map [{:a 10} {:a 9} {:a \"How rare! A string!\"}] :a)");

  // Then
  ASSERT_EQ(result->to_string(), "[10 9 \"How rare! A string!\"]");
}

TEST_F(MapFunction, maps_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((map "ab" str))")->to_string(), R"(["a" "b"])");
}

TEST_F(MapFunction, maps_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(map {:a 1 :b 2} str)")->to_string(), R"([":a" "1" ":b" "2"])");
}

TEST_F(MapFunction, maps_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(map values (fn [n] (* n 2)))")->to_string(), "[2 4 6]");
}

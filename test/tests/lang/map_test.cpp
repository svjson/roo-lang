
#include <gtest/gtest.h>
#include <lisple/adapter.h>
#include "runtime_fixture.h"


using MapFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * MapFunction - (map [...] transform-fn)
 * ======================================================================
 */

TEST_F(MapFunction, map_single_array_of_numbers)
{
  // Given
  auto retval = runtime.eval("(map [1 2 3] (fn [num] (* num 2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6]");
}

TEST_F(MapFunction, map_single_vector_int)
{
  // Given
  std::vector<int> int_v{1, 2, 3, 4, 5, 6};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(map wrapped-vec (fn [num] (* num 2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6 8 10 12]");
}

TEST_F(MapFunction, map_two_arrays_of_numbers)
{
  // Given
  auto retval = runtime.eval("(map [1 2 3] [30 20 10] (fn [n1 n2] (+ n1 n2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[31 22 13]");
}

TEST_F(MapFunction, map_two_seqs_of_different_types)
{
  // Given
  std::vector<int> int_v{1, 2, 3};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(map wrapped-vec [30 20 10] (fn [n1 n2] (+ n1 n2)))");

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

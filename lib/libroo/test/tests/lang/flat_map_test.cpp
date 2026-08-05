#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FlatMapFunction = RooTest::RuntimeTestFixture;

TEST_F(FlatMapFunction, maps_and_flattens_seq_results)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map [1 2 3] (fn [n] [n (* n 10)]))")->to_string(),
            "[1 10 2 20 3 30]");
}

TEST_F(FlatMapFunction, includes_scalar_results_directly)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map [[1 2] 3 [4 5]] identity)")->to_string(),
            "[1 2 3 4 5]");
}

TEST_F(FlatMapFunction, accepts_function_first_arg_order)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map (fn [n] [n n]) [1 2 3])")->to_string(),
            "[1 1 2 2 3 3]");
}

TEST_F(FlatMapFunction, treats_nil_as_sequence_in_sequence_first_arg_order)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map nil identity)")->to_string(), "[]");
}

TEST_F(FlatMapFunction, treats_nil_as_sequence_in_function_first_arg_order)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map identity nil)")->to_string(), "[]");
}

TEST_F(FlatMapFunction, empty_collection_yields_empty)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map [] (fn [n] [n n]))")->to_string(), "[]");
}

TEST_F(FlatMapFunction, keyword_used_as_property_getter)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map [{:xs [1 2]} {:xs [3]} {:xs [4 5]}] :xs)")->to_string(),
            "[1 2 3 4 5]");
}

TEST_F(FlatMapFunction, empty_inner_seqs_contribute_nothing)
{
  // Then
  EXPECT_EQ(runtime.eval("(flat-map [1 2 3] (fn [n] (if (even? n) [n] [])))")->to_string(),
            "[2]");
}

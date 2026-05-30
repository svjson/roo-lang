
#include <vector>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/adapter.h>
#include <roo/runtime/seq.h>

using RemoveFirstFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * RemoveFirstFunction - (remove-first pred seq)
 * Yields a new seq with the first element satisfying pred omitted.
 * ======================================================================
 */

TEST_F(RemoveFirstFunction, removes_first_matching_element)
{
  // Given
  auto result = runtime.eval("(remove-first (fn [x] (= x 2)) [1 2 3 2 4])");

  // Then
  ASSERT_EQ(Roo::count(*result), 4);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 3);
  EXPECT_EQ(result->elements().at(2)->i64(), 2);
  EXPECT_EQ(result->elements().at(3)->i64(), 4);
}

TEST_F(RemoveFirstFunction, removes_first_even_element)
{
  // Given
  auto result = runtime.eval("(remove-first even? [1 2 3 4 5])");

  // Then
  ASSERT_EQ(Roo::count(*result), 4);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 3);
  EXPECT_EQ(result->elements().at(2)->i64(), 4);
  EXPECT_EQ(result->elements().at(3)->i64(), 5);
}

TEST_F(RemoveFirstFunction, no_match_returns_original)
{
  // Given
  auto result = runtime.eval("(remove-first even? [1 3 5 7])");

  // Then
  ASSERT_EQ(Roo::count(*result), 4);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 3);
  EXPECT_EQ(result->elements().at(2)->i64(), 5);
  EXPECT_EQ(result->elements().at(3)->i64(), 7);
}

TEST_F(RemoveFirstFunction, empty_sequence_returns_empty)
{
  // Given
  auto result = runtime.eval("(remove-first even? [])");

  // Then
  ASSERT_EQ(Roo::count(*result), 0);
}

TEST_F(RemoveFirstFunction, removes_only_first_of_multiple_matches)
{
  // Given
  auto result = runtime.eval("(remove-first even? [2 4 6 8])");

  // Then
  ASSERT_EQ(Roo::count(*result), 3);
  EXPECT_EQ(result->elements().at(0)->i64(), 4);
  EXPECT_EQ(result->elements().at(1)->i64(), 6);
  EXPECT_EQ(result->elements().at(2)->i64(), 8);
}

TEST_F(RemoveFirstFunction, removes_from_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((remove-first "abcb" (fn [c] (= c 'b'))))")->to_string(),
            "['a' 'c' 'b']");
}

TEST_F(RemoveFirstFunction, removes_from_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(remove-first {:a 1 :b 2} keyword?)")->to_string(), "[1 :b 2]");
}

TEST_F(RemoveFirstFunction, removes_from_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(remove-first values even?)")->to_string(), "[1 3]");
}

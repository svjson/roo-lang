
#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SomeFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * SomeFunction - (some? seq pred)
 * Returns true if at least one element satisfies the predicate.
 * ======================================================================
 */

TEST_F(SomeFunction, returns_true_when_at_least_one_element_matches)
{
  // Given
  auto result = runtime.eval("(some? [2 4 6] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_TRUE);
}

TEST_F(SomeFunction, returns_true_when_first_element_matches)
{
  // Given
  auto result = runtime.eval("(some? [2 1 3 5] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_TRUE);
}

TEST_F(SomeFunction, returns_true_when_last_element_matches)
{
  // Given
  auto result = runtime.eval("(some? [1 3 5 4] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_TRUE);
}

TEST_F(SomeFunction, returns_false_when_no_element_matches)
{
  // Given
  auto result = runtime.eval("(some? [1 3 5 7] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_FALSE);
}

TEST_F(SomeFunction, returns_false_for_empty_sequence)
{
  // Given
  auto result = runtime.eval("(some? [] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_FALSE);
}

TEST_F(SomeFunction, works_with_custom_predicate)
{
  // Given
  auto found = runtime.eval(R"((some? ["hi" "hey" "hello"] (fn [s] (= 3 (count s)))))");
  auto not_found = runtime.eval(R"((some? ["hi" "hello"] (fn [s] (= 3 (count s)))))");

  // Then
  EXPECT_EQ(*found, *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*not_found, *Lisple::Constant::BOOL_FALSE);
}

TEST_F(SomeFunction, searches_string_as_char_sequence)
{
  EXPECT_EQ(*runtime.eval(R"((some? "abc" (fn [c] (= c 'b'))))"),
            *Lisple::Constant::BOOL_TRUE);
}

TEST_F(SomeFunction, searches_map_as_interleaved_sequence)
{
  EXPECT_EQ(*runtime.eval("(some? {:a 1 :b 2} keyword?)"), *Lisple::Constant::BOOL_TRUE);
}

TEST_F(SomeFunction, searches_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(some? values even?)"), *Lisple::Constant::BOOL_TRUE);
}

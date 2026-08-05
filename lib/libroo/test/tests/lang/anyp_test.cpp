
#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AnyFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * AnyFunction - (any? seq pred)
 * Returns true if at least one element satisfies the predicate.
 * ======================================================================
 */

TEST_F(AnyFunction, returns_true_when_at_least_one_element_matches)
{
  // Given
  auto result = runtime.eval("(any? [2 4 6] even?)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(AnyFunction, returns_true_when_first_element_matches)
{
  // Given
  auto result = runtime.eval("(any? [2 1 3 5] even?)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(AnyFunction, returns_true_when_last_element_matches)
{
  // Given
  auto result = runtime.eval("(any? [1 3 5 4] even?)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(AnyFunction, returns_false_when_no_element_matches)
{
  // Given
  auto result = runtime.eval("(any? [1 3 5 7] even?)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_FALSE);
}

TEST_F(AnyFunction, returns_false_for_empty_sequence)
{
  // Given
  auto result = runtime.eval("(any? [] even?)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_FALSE);
}

TEST_F(AnyFunction, works_with_custom_predicate)
{
  // Given
  auto found = runtime.eval(R"((any? ["hi" "hey" "hello"] (fn [s] (= 3 (count s)))))");
  auto not_found = runtime.eval(R"((any? ["hi" "hello"] (fn [s] (= 3 (count s)))))");

  // Then
  EXPECT_EQ(*found, *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*not_found, *Roo::Constant::BOOL_FALSE);
}

TEST_F(AnyFunction, searches_string_as_char_sequence)
{
  EXPECT_EQ(*runtime.eval(R"((any? "abc" (fn [c] (= c 'b'))))"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(AnyFunction, searches_map_as_interleaved_sequence)
{
  EXPECT_EQ(*runtime.eval("(any? {:a 1 :b 2} keyword?)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(AnyFunction, searches_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(any? values even?)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(AnyFunction, accepts_predicate_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(any? even? [1 3 4])"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(AnyFunction, treats_nil_as_sequence_in_sequence_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(any? nil even?)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(AnyFunction, treats_nil_as_sequence_in_predicate_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(any? even? nil)"), *Roo::Constant::BOOL_FALSE);
}

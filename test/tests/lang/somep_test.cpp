
#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ======================================================================
 * SomeFunction - (some? seq pred)
 * Returns true if at least one element satisfies the predicate.
 * ======================================================================
 */

TEST(SomeFunction, returns_true_when_at_least_one_element_matches)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(some? [2 4 6] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_TRUE);
}

TEST(SomeFunction, returns_true_when_first_element_matches)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(some? [2 1 3 5] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_TRUE);
}

TEST(SomeFunction, returns_true_when_last_element_matches)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(some? [1 3 5 4] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_TRUE);
}

TEST(SomeFunction, returns_false_when_no_element_matches)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(some? [1 3 5 7] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_FALSE);
}

TEST(SomeFunction, returns_false_for_empty_sequence)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(some? [] even?)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::BOOL_FALSE);
}

TEST(SomeFunction, works_with_custom_predicate)
{
  // Given
  Lisple::Runtime runtime;

  // When (look for a string of length 3)
  auto found = runtime.eval(R"((some? ["hi" "hey" "hello"] (fn [s] (= 3 (count s)))))");
  auto not_found = runtime.eval(R"((some? ["hi" "hello"] (fn [s] (= 3 (count s)))))");

  // Then
  EXPECT_EQ(*found, *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*not_found, *Lisple::Constant::BOOL_FALSE);
}

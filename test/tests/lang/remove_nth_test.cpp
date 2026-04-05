
#include <lisple/runtime.h>
#include <lisple/runtime/seq.h>

#include <gtest/gtest.h>

/*
 * ======================================================================
 * RemoveNthFunction - (remove-nth seq n)
 * Returns a copy of seq with the element at index n omitted.
 * ======================================================================
 */

TEST(RemoveNthFunction, removes_middle_element)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(remove-nth [1 2 3 4 5] 2)");

  // Then
  ASSERT_EQ(Lisple::count(*result), 4);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 2);
  EXPECT_EQ(result->elements().at(2)->i64(), 4);
  EXPECT_EQ(result->elements().at(3)->i64(), 5);
}

TEST(RemoveNthFunction, removes_first_element)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(remove-nth [1 2 3] 0)");

  // Then
  ASSERT_EQ(Lisple::count(*result), 2);
  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 3);
}

TEST(RemoveNthFunction, removes_last_element)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(remove-nth [1 2 3] 2)");

  // Then
  ASSERT_EQ(Lisple::count(*result), 2);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 2);
}

TEST(RemoveNthFunction, out_of_bounds_index_returns_original)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(remove-nth [1 2 3] 5)");

  // Then
  ASSERT_EQ(Lisple::count(*result), 3);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 2);
  EXPECT_EQ(result->elements().at(2)->i64(), 3);
}

TEST(RemoveNthFunction, negative_index_returns_original)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(remove-nth [1 2 3] -1)");

  // Then
  ASSERT_EQ(Lisple::count(*result), 3);
}

TEST(RemoveNthFunction, does_not_mutate_original)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def original [10 20 30 40])");

  // When
  runtime.eval("(remove-nth original 1)");

  // Then (original is unchanged)
  auto original = runtime.lookup_value("original");
  ASSERT_EQ(Lisple::count(*original), 4);
}

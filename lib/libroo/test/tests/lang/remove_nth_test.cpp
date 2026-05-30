
#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/exception.h>
#include <roo/runtime/seq.h>

using RemoveNthFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * RemoveNthFunction - (remove-nth seq n)
 * Returns a copy of seq with the element at index n omitted.
 * ======================================================================
 */

TEST_F(RemoveNthFunction, removes_middle_element)
{
  // Given
  auto result = runtime.eval("(remove-nth [1 2 3 4 5] 2)");

  // Then
  ASSERT_EQ(Roo::count(*result), 4);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 2);
  EXPECT_EQ(result->elements().at(2)->i64(), 4);
  EXPECT_EQ(result->elements().at(3)->i64(), 5);
}

TEST_F(RemoveNthFunction, removes_first_element)
{
  // Given
  auto result = runtime.eval("(remove-nth [1 2 3] 0)");

  // Then
  ASSERT_EQ(Roo::count(*result), 2);
  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 3);
}

TEST_F(RemoveNthFunction, removes_last_element)
{
  // Given
  auto result = runtime.eval("(remove-nth [1 2 3] 2)");

  // Then
  ASSERT_EQ(Roo::count(*result), 2);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 2);
}

TEST_F(RemoveNthFunction, out_of_bounds_index_returns_original)
{
  // Given
  auto result = runtime.eval("(remove-nth [1 2 3] 5)");

  // Then
  ASSERT_EQ(Roo::count(*result), 3);
  EXPECT_EQ(result->elements().at(0)->i64(), 1);
  EXPECT_EQ(result->elements().at(1)->i64(), 2);
  EXPECT_EQ(result->elements().at(2)->i64(), 3);
}

TEST_F(RemoveNthFunction, negative_index_returns_original)
{
  // Given
  auto result = runtime.eval("(remove-nth [1 2 3] -1)");

  // Then
  ASSERT_EQ(Roo::count(*result), 3);
}

TEST_F(RemoveNthFunction, does_not_mutate_original)
{
  // Given
  runtime.eval("(def original [10 20 30 40])");

  // When
  runtime.eval("(remove-nth original 1)");

  // Then (original is unchanged)
  auto original = runtime.lookup("original");
  ASSERT_EQ(Roo::count(*original), 4);
}

TEST_F(RemoveNthFunction, remove_nth_bang_rejects_map_target)
{
  EXPECT_THROW(runtime.eval("(remove-nth! {:a 1} 0)"), Roo::InvocationException);
}

#include <lisple/adapter.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ===================================
 * FlattenFunction - (flatten [...] [...])
 * ===================================
 */

TEST(FlattenFunction, array_of_array)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(flatten [[1 2 3] [4 5 6] [7 8 9]])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST(FlattenFunction, nested_arrays)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(flatten [1 [2 3] [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST(FlattenFunction, nested_vector_int_in_array)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v{2, 3};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(flatten [1 wrapped-vec [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

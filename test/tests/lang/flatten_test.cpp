#include <lisple/adapter.h>
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using FlattenFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * FlattenFunction - (flatten [...] [...])
 * ===================================
 */

TEST_F(FlattenFunction, array_of_array)
{
  // Given
  auto retval = runtime.eval("(flatten [[1 2 3] [4 5 6] [7 8 9]])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST_F(FlattenFunction, nested_arrays)
{
  // Given
  auto retval = runtime.eval("(flatten [1 [2 3] [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST_F(FlattenFunction, maps_are_leaf_values)
{
  // Given
  auto retval = runtime.eval("(flatten [[{:x 1 :y 2}] [{:x 3 :y 4}]])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[{:x 1 :y 2} {:x 3 :y 4}]"));
}

TEST_F(FlattenFunction, nested_vector_int_in_array)
{
  // Given
  std::vector<int> int_v{2, 3};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(flatten [1 wrapped-vec [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

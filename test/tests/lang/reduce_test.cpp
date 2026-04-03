
#include <lisple/adapter.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ======================================================================
 * ReduceFunction - (reduce [...] reducer-fn)
 * ======================================================================
 */

TEST(ReduceFunction, reduce_array_of_num)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(reduce [128 64 32 16 8 4 2 1] 0 +)");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(255));
}

TEST(ReduceFunction, reduce_vector_int)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v{128, 64, 32, 16, 8, 4, 2, 1};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(reduce wrapped-vec 0 +)");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(255));
}

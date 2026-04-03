
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(GetFunction, get_from_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get my-map :b)");

  // Then
  ASSERT_EQ(result->i64(), 2);
}

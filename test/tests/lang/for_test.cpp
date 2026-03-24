#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(ForMacro, transform_vector_of_int)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-vector [1 2 3 4 5 6])");

  // When
  auto retval = runtime.eval("(for [num my-vector] (* num 2))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6 8 10 12]");
}

TEST(ForMacro, with_map_destructuring)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-vector [{:a 1 :b 10} {:a 5 :b 4}])");

  // When
  auto retval = runtime.eval("(for [{:keys [a b]} my-vector] (+ a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 9]");
}

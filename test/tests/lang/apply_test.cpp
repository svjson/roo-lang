
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(ApplyFunction, apply_concat)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(apply concat [[1 2 3] [4 5 6]])");

  // Then
  ASSERT_EQ(retval->to_string(), "[1 2 3 4 5 6]");
}

TEST(ApplyFunction, apply_dynamic)
{
  // Given
  Lisple::Runtime runtime;

  // When
  runtime.eval("(def function-name 'max)");
  auto result = runtime.eval("(apply (resolve function-name) [10 8 17 4 0])");

  // Then
  ASSERT_EQ(result->to_string(), "17");
}

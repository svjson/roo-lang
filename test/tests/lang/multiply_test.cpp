
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(MultiplyFunction, small_int_multiplication)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(* 10 5)");

  // Then
  EXPECT_EQ(result->to_string(), "50");
}

TEST(MultiplyFunction, multiplication_i32_to_i64_result)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(* 123456789 1103515245)");

  // Then
  EXPECT_EQ(result->to_string(), "136236448760248305");
}

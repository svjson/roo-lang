
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using MultiplyFunction = LispleTest::RuntimeTestFixture;
TEST_F(MultiplyFunction, small_int_multiplication)
{
  // Given
  auto result = runtime.eval("(* 10 5)");

  // Then
  EXPECT_EQ(result->to_string(), "50");
}

TEST_F(MultiplyFunction, multiplication_i32_to_i64_result)
{
  // Given
  auto result = runtime.eval("(* 123456789 1103515245)");

  // Then
  EXPECT_EQ(result->to_string(), "136236448760248305");
}

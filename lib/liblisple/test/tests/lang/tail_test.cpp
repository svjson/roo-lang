#include "runtime_fixture.h"

#include <gtest/gtest.h>


using TailFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * TailFunction - (tail [...])
 * ===================================
 */

TEST_F(TailFunction, tail_of_vector)
{
  // Given
  runtime.eval("(def my-vector ['a' 'b' 'c'])");

  // When
  auto result_ref = runtime.eval("(tail my-vector)");
  auto result_lit = runtime.eval("(tail ['a' 'b' 'c'])");

  // Then
  EXPECT_EQ(result_ref->to_string(), "['b' 'c']");
  EXPECT_EQ(result_lit->to_string(), "['b' 'c']");
}

TEST_F(TailFunction, single_element)
{
  // When
  auto result = runtime.eval("(tail ['a'])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("[]"));
}

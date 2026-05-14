#include <lisple/adapter.h>
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using TailFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * TailFunction - (tail [...])
 * ===================================
 */

TEST_F(TailFunction, tail_of_array)
{
  // Given
  runtime.eval("(def my-array ['a' 'b' 'c'])");

  std::vector<int> int_v{50, 100, 90};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result_ref = runtime.eval("(tail my-array)");
  auto result_lit = runtime.eval("(tail ['a' 'b' 'c'])");
  auto result_wrapped = runtime.eval("(tail wrapped-vec)");

  // Then
  EXPECT_EQ(result_ref->to_string(), "['b' 'c']");
  EXPECT_EQ(result_lit->to_string(), "['b' 'c']");
  EXPECT_EQ(result_wrapped->to_string(), "[100 90]");
}

TEST_F(TailFunction, single_element)
{
  // Given
  std::vector<int> int_v{50};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result = runtime.eval("(tail ['a'])");
  auto result_wrapped = runtime.eval("(tail wrapped-vec)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("[]"));
  EXPECT_EQ(*result_wrapped, *runtime.eval("[]"));
}

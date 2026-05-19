#include <vector>

#include <lisple/adapter.h>

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

TEST_F(TailFunction, tail_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((tail "abc"))")->to_string(), "['b' 'c']");
}

TEST_F(TailFunction, tail_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(tail {:a 1 :b 2})")->to_string(), "[1 :b 2]");
}

TEST_F(TailFunction, tail_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(tail values)")->to_string(), "[2 3]");
}

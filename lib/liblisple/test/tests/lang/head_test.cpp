
#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using HeadFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * HeadFunction - (head [...])
 * ===================================
 */

TEST_F(HeadFunction, head_of_seq)
{
  // Given
  runtime.eval("(def my-vector ['a' 'b' 'c'])");

  // When
  auto result_ref = runtime.eval("(head my-vector)");
  auto result_lit = runtime.eval("(head ['a' 'b' 'c'])");

  // Then
  EXPECT_EQ(*result_ref, *Lisple::Value::character('a'));
  EXPECT_EQ(*result_lit, *Lisple::Value::character('a'));
}

TEST_F(HeadFunction, single_element)
{
  // When
  auto result_lit = runtime.eval("(head ['a'])");

  // Then
  EXPECT_EQ(*result_lit, *Lisple::Value::character('a'));
}

TEST_F(HeadFunction, head_vector)
{
  // Given
  EXPECT_EQ(runtime.eval("(head [8 9 10])")->to_string(), "8");
}

TEST_F(HeadFunction, head_nil)
{
  // Given
  EXPECT_EQ(runtime.eval("(head nil)")->to_string(), "nil");
}

TEST_F(HeadFunction, head_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((head "abc"))")->to_string(), "'a'");
}

TEST_F(HeadFunction, head_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(head {:a 1 :b 2})")->to_string(), ":a");
}

TEST_F(HeadFunction, head_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(head values)")->to_string(), "1");
}

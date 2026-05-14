
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
  runtime.eval("(def my-array ['a' 'b' 'c'])");

  // When
  auto result_ref = runtime.eval("(head my-array)");
  auto result_lit = runtime.eval("(head ['a' 'b' 'c'])");

  // Then
  EXPECT_EQ(*result_ref, *Lisple::RTValue::character('a'));
  EXPECT_EQ(*result_lit, *Lisple::RTValue::character('a'));
}

TEST_F(HeadFunction, single_element)
{
  // When
  auto result_lit = runtime.eval("(head ['a'])");

  // Then
  EXPECT_EQ(*result_lit, *Lisple::RTValue::character('a'));
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

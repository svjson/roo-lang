
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
  runtime.eval("(def my-array ['a' 'b' 'c'])");

  std::vector<int> int_v{50, 100, 90};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result_ref = runtime.eval("(head my-array)");
  auto result_lit = runtime.eval("(head ['a' 'b' 'c'])");
  auto result_wrapped = runtime.eval("(head wrapped-vec)");

  // Then
  EXPECT_EQ(*result_ref, *Lisple::RTValue::character('a'));
  EXPECT_EQ(*result_lit, *Lisple::RTValue::character('a'));
  EXPECT_EQ(*result_wrapped, *Lisple::RTValue::number(50))
    << result_wrapped->to_string() << " vs " << Lisple::Number(50).to_string();
}

TEST_F(HeadFunction, single_element)
{
  // Given
  std::vector<int> int_v{9};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result_lit = runtime.eval("(head ['a'])");
  auto result_wrapped = runtime.eval("(head wrapped-vec)");

  // Then
  EXPECT_EQ(*result_lit, *Lisple::RTValue::character('a'));
  EXPECT_EQ(*result_wrapped, *Lisple::RTValue::number(9));
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

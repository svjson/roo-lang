
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ForIndexedForm = RooTest::RuntimeTestFixture;

TEST_F(ForIndexedForm, index_and_value_bound_per_iteration)
{
  // When
  auto retval = runtime.eval("(for-indexed [i num [1 2 3 4]] (* i num))");

  // Then
  ASSERT_EQ(retval->to_string(), "[0 2 6 12]");
}

TEST_F(ForIndexedForm, index_only_without_using_value)
{
  // When
  auto retval = runtime.eval("(for-indexed [i x [\"a\" \"b\" \"c\"]] i)");

  // Then
  ASSERT_EQ(retval->to_string(), "[0 1 2]");
}

TEST_F(ForIndexedForm, two_collections_with_index)
{
  // When
  auto retval = runtime.eval("(for-indexed [i a [1 2 3] b [10 20 30]] (str i \":\" (+ a b)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[\"0:11\" \"1:22\" \"2:33\"]");
}

TEST_F(ForIndexedForm, stops_at_shortest_collection)
{
  // When -- second collection is shorter
  auto retval = runtime.eval("(for-indexed [i a [1 2 3] b [10 20]] (+ i a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 23]");
}

TEST_F(ForIndexedForm, empty_body_returns_nil_values)
{
  auto result = runtime.eval("(for-indexed [i n [1 2 3]])");

  ASSERT_TRUE(result.get());
  ASSERT_EQ(result->to_string(), "[nil nil nil]");
  for (auto& value : result->elements())
  {
    ASSERT_TRUE(value.get());
    ASSERT_EQ(value, Roo::Constant::NIL);
  }
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ForForm = RooTest::RuntimeTestFixture;
TEST_F(ForForm, transform_vector_of_int)
{
  // Given
  runtime.eval("(def my-vector [1 2 3 4 5 6])");

  // When
  auto retval = runtime.eval("(for [num my-vector] (* num 2))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6 8 10 12]");
}

TEST_F(ForForm, with_map_destructuring)
{
  // Given
  runtime.eval("(def my-vector [{:a 1 :b 10} {:a 5 :b 4}])");

  // When
  auto retval = runtime.eval("(for [{:keys [a b]} my-vector] (+ a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 9]");
}

TEST_F(ForForm, each_char_in_string)
{
  // Given
  auto retval = runtime.eval("(for [ch \"Beer Gnome!\"] ch)");

  // Then
  ASSERT_EQ(retval->to_string(), "['B' 'e' 'e' 'r' ' ' 'G' 'n' 'o' 'm' 'e' '!']");
}

TEST_F(ForForm, two_collections_iterated_in_lockstep)
{
  // When
  auto retval = runtime.eval("(for [a [1 2 3] b [10 20 30]] (+ a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 22 33]");
}

TEST_F(ForForm, stops_at_shortest_collection)
{
  // When -- second collection is shorter
  auto retval = runtime.eval("(for [a [1 2 3 4 5] b [10 20]] (+ a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 22]");
}

TEST_F(ForForm, three_collections_in_lockstep)
{
  // When
  auto retval = runtime.eval("(for [a [1 2 3] b [10 20 30] c [100 200 300]] (+ a b c))");

  // Then
  ASSERT_EQ(retval->to_string(), "[111 222 333]");
}

TEST_F(ForForm, multi_binding_with_map_in_body)
{
  // When -- combine keys and values from separate collections
  auto retval = runtime.eval("(for [k [:a :b :c] v [1 2 3]] {k v})");

  // Then
  ASSERT_EQ(Roo::count(*retval), 3);
}

TEST_F(ForForm, map_destructuring_in_multi_binding)
{
  // When -- destructure maps from first coll, plain value from second
  auto retval = runtime.eval(
    "(for [{:keys [a b]} [{:a 1 :b 10} {:a 2 :b 20}] n [100 200]] (+ a b n))");

  // Then
  ASSERT_EQ(retval->to_string(), "[111 222]");
}

TEST_F(ForForm, vector_destructuring_in_multi_binding)
{
  // When -- destructure pairs from first coll, plain value from second
  auto retval = runtime.eval(
    "(for [[x y] [[1 2] [3 4]] n [10 20]] (+ x y n))");

  // Then
  ASSERT_EQ(retval->to_string(), "[13 27]");
}

TEST_F(ForForm, empty_body_returns_nil_values)
{
  auto result = runtime.eval("(for [n [1 2 3]])");

  ASSERT_TRUE(result.get());
  ASSERT_EQ(result->to_string(), "[nil nil nil]");
  for (auto& value : result->elements())
  {
    ASSERT_TRUE(value.get());
    ASSERT_EQ(value, Roo::Constant::NIL);
  }
}

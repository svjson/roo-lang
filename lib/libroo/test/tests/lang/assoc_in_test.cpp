
#include <roo/exception.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AssocInFunction = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(AssocInFunction, add_key_to_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:c] 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), runtime.eval("{:a 1 :b 2}")->to_string());
}

TEST_F(AssocInFunction, add_key_to_nested_map)
{
  // Given
  runtime.eval(R"((def my-map {:a 1 :b 2 :c {:key1 "val"}}))");

  // When
  auto result = runtime.eval("(assoc-in my-map [:c :key2] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval(R"({:a 1 :b 2 :c {:key1 "val" :key2 44}})"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2 :c {:key1 \"val\"}}")->to_string());
}

TEST_F(AssocInFunction, add_string_key_to_nested_map)
{
  // Given
  runtime.eval(R"((def my-map {:a 1 :b 2 :c {:key1 "val"}}))");

  // When
  auto result = runtime.eval(R"((assoc-in my-map [:c "str-key"] 44))");

  // Then
  EXPECT_EQ(*result, *runtime.eval(R"({:a 1 :b 2 :c {:key1 "val" "str-key" 44}})"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval(R"({:a 1 :b 2 :c {:key1 "val"}})")->to_string());
}

TEST_F(AssocInFunction, add_referenced_string_key_to_nested_map)
{
  // Given
  runtime.eval(R"(
    (def my-key "str-key")
    (def my-map {:a 1 :b 2 :c {:key1 "val"}}))");

  // When
  auto result = runtime.eval(R"((assoc-in my-map [:c my-key] 44))");

  // Then
  EXPECT_EQ(*result, *runtime.eval(R"({:a 1 :b 2 :c {:key1 "val" "str-key" 44}})"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval(R"({:a 1 :b 2 :c {:key1 "val"}})")->to_string());
}

TEST_F(AssocInFunction, creates_missing_intermediate_maps)
{
  // Given
  auto result = runtime.eval("(assoc-in {} [:a :b :c] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a {:b {:c 44}}}"));
}

TEST_F(AssocInFunction, replaces_intermediate_nil_with_map)
{
  // Given
  runtime.eval("(def my-map {:a nil})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:a :b] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a {:b 44}}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:a nil}");
}

TEST_F(AssocInFunction, replace_key_in_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:b] 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), runtime.eval("{:a 1 :b 2}")->to_string());
}

TEST_F(AssocInFunction, replace_key_when_same_value_occurs_before_key)
{
  // Given
  runtime.eval("(def my-map {:id :type :type :type/plain})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:type] :type/updated)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:id :type :type :type/updated}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:id :type :type :type/plain}"));
}

TEST_F(AssocInFunction, replace_element_in_vector)
{
  // Given
  runtime.eval("(def my-map {:vec ['a' 'b' 'c']})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:vec 1] 'x')");

  // Then
  EXPECT_EQ(result->to_string(), "{:vec ['a' 'x' 'c']}");
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:vec ['a' 'b' 'c']}");
}

TEST_F(AssocInFunction, creates_map_in_nil_vector_slot)
{
  // Given
  runtime.eval("(def my-map {:vec []})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:vec 2 :x] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:vec [nil nil {:x 44}]}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:vec []}");
}

TEST_F(AssocInFunction, add_and_replace_multiple)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :nested {:x 0}})");

  // When
  auto result =
    runtime.eval("(assoc-in my-map [:b] 10 [:nested :y] 3 [:c] \"some string\")");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10 :nested {:x 0 :y 3} :c \"some string\"}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:a 1 :b 2 :nested {:x 0}}");
}

TEST_F(AssocInFunction, throws_on_incomplete_path_value_chain)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When/Then
  EXPECT_THAT(
    [this]() { runtime.eval("(assoc-in my-map [:b] 10 [:c])"); },
    ThrowsMessage<Roo::InvocationException>(HasSubstr("No value given for path '[:c] '")));
}

TEST_F(AssocInFunction, throws_on_non_sequence_path)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When/Then
  EXPECT_THAT(
    [this]() { runtime.eval("(assoc-in my-map :b 10)"); },
    ThrowsMessage<Roo::TypeError>(HasSubstr("Path for assoc-in must be a sequence")));
}

TEST_F(AssocInFunction, throws_when_non_nil_scalar_blocks_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT(
    [this]() { runtime.eval("(assoc-in my-map [:a :b] 10)"); },
    ThrowsMessage<Roo::TypeError>(HasSubstr("assoc-in cannot traverse through 1")));
}

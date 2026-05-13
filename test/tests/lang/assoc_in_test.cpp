
#include <lisple/exception.h>
#include <lisple/runtime.h>

#include "gmock/gmock.h"
#include <gtest/gtest.h>

using namespace ::testing;

TEST(AssocInFunction, add_key_to_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:c] 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2}")->to_string());
}

TEST(AssocInFunction, add_key_to_nested_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c {:key1 \"val\"}})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:c :key2] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c {:key1 \"val\" :key2 44}}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2 :c {:key1 \"val\"}}")->to_string());
}

TEST(AssocInFunction, creates_missing_intermediate_maps)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(assoc-in {} [:a :b :c] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a {:b {:c 44}}}"));
}

TEST(AssocInFunction, replaces_intermediate_nil_with_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a nil})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:a :b] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a {:b 44}}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:a nil}");
}

TEST(AssocInFunction, replace_key_in_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:b] 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2}")->to_string());
}

TEST(AssocInFunction, replace_element_in_vector)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:vec ['a' 'b' 'c']})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:vec 1] 'x')");

  // Then
  EXPECT_EQ(result->to_string(), "{:vec ['a' 'x' 'c']}");
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:vec ['a' 'b' 'c']}");
}

TEST(AssocInFunction, creates_map_in_nil_vector_slot)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:vec []})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:vec 2 :x] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:vec [nil nil {:x 44}]}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:vec []}");
}

TEST(AssocInFunction, add_and_replace_multiple)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :nested {:x 0}})");

  // When
  auto result = runtime.eval("(assoc-in my-map [:b] 10 [:nested :y] 3 [:c] \"some string\")");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10 :nested {:x 0 :y 3} :c \"some string\"}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:a 1 :b 2 :nested {:x 0}}");
}

TEST(AssocInFunction, throws_on_incomplete_path_value_chain)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When/Then
  EXPECT_THAT([&runtime]() { runtime.eval("(assoc-in my-map [:b] 10 [:c])"); },
              ThrowsMessage<Lisple::InvocationException>(
                HasSubstr("No value given for path '[:c] '")));
}

TEST(AssocInFunction, throws_on_non_sequence_path)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When/Then
  EXPECT_THAT([&runtime]() { runtime.eval("(assoc-in my-map :b 10)"); },
              ThrowsMessage<Lisple::TypeError>(
                HasSubstr("Path for assoc-in must be a sequence")));
}

TEST(AssocInFunction, throws_when_non_nil_scalar_blocks_path)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT([&runtime]() { runtime.eval("(assoc-in my-map [:a :b] 10)"); },
              ThrowsMessage<Lisple::TypeError>(
                HasSubstr("assoc-in cannot traverse through 1")));
}

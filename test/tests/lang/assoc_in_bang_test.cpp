#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(AssocInBangFunction, add_key_to_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in! my-map [:c] 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2 :c 3}")->to_string());
}

TEST(AssocInBangFunction, add_key_to_nested_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c {:key1 \"val\"}})");

  // When
  auto result = runtime.eval("(assoc-in! my-map [:c :key2] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c {:key1 \"val\" :key2 44}}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2 :c {:key1 \"val\" :key2 44}}")->to_string());
}

TEST(AssocInBangFunction, replace_key_in_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in! my-map [:b] 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(),
            runtime.eval("{:a 1 :b 10}")->to_string());
}

#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(AssocBangFunction, add_key_to_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(runtime.lookup(Lisple::Word("my-map"))->to_string(),
            runtime.eval("{:a 1 :b 2 :c 3}")->to_string());
}

TEST(AssocBangFunction, replace_key_in_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :b 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10}"));
  EXPECT_EQ(runtime.lookup(Lisple::Word("my-map"))->to_string(),
            runtime.eval("{:a 1 :b 10}")->to_string());
}

TEST(AssocBangFunction, add_and_replace_multiple)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :b 10 :c 3 :d \"some string\")");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}"));
  EXPECT_EQ(runtime.lookup(Lisple::Word("my-map"))->to_string(),
            runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}")->to_string());
}

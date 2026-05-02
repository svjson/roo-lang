#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(UpdateFunction, update_existing_map_key)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:count 2 :other 99})");

  // When
  auto result = runtime.eval("(update my-map :count (fn [x] (+ x 1)))");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:count 3 :other 99}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:count 2 :other 99}");
}

TEST(UpdateFunction, update_missing_map_key_with_nil_current_value)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1})");

  // When
  auto result = runtime.eval("(update my-map :b (fn [x] (if (nil? x) 44 (+ x 1))))");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 44}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:a 1}");
}

TEST(UpdateFunction, passes_extra_args_to_update_function)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:count 2})");

  // When
  auto result = runtime.eval("(update my-map :count [+ 10 5])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:count 17}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:count 2}");
}

TEST(UpdateFunction, update_sequence_by_index)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-vec [1 2 3])");

  // When
  auto result = runtime.eval("(update my-vec 1 [* 10])");

  // Then
  EXPECT_EQ(result->to_string(), "[1 20 3]");
  EXPECT_EQ(runtime.lookup_value("my-vec")->to_string(), "[1 2 3]");
}

TEST(UpdateFunction, update_multiple_keys_in_one_call)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(update my-map :a (fn [x] (+ x 1)) :b [+ 10])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 2 :b 12}"));
  EXPECT_EQ(runtime.lookup_value("my-map")->to_string(), "{:a 1 :b 2}");
}

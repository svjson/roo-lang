#include <lisple/exception.h>
#include "runtime_fixture.h"

#include "gmock/gmock.h"
#include <gtest/gtest.h>


using UpdateInFunction = LispleTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(UpdateInFunction, update_existing_nested_map_key)
{
  // Given
  runtime.eval("(def my-map {:nested {:count 2} :other 99})");

  // When
  auto result = runtime.eval("(update-in my-map [:nested :count] (fn [x] (+ x 1)))");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:nested {:count 3} :other 99}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:nested {:count 2} :other 99}");
}

TEST_F(UpdateInFunction, update_missing_nested_key_with_nil_current_value)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When
  auto result = runtime.eval("(update-in my-map [:b :c] (fn [x] (if (nil? x) 44 (+ x 1))))");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b {:c 44}}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:a 1}");
}

TEST_F(UpdateInFunction, passes_extra_args_to_update_function)
{
  // Given
  runtime.eval("(def my-map {:nested {:count 2}})");

  // When
  auto result = runtime.eval("(update-in my-map [:nested :count] [+ 10 5])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:nested {:count 17}}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:nested {:count 2}}");
}

TEST_F(UpdateInFunction, update_nested_sequence_by_index)
{
  // Given
  runtime.eval("(def my-map {:vec [1 2 3]})");

  // When
  auto result = runtime.eval("(update-in my-map [:vec 1] [* 10])");

  // Then
  EXPECT_EQ(result->to_string(), "{:vec [1 20 3]}");
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:vec [1 2 3]}");
}

TEST_F(UpdateInFunction, update_root_sequence_by_path)
{
  // Given
  runtime.eval("(def my-vec [1 2 3])");

  // When
  auto result = runtime.eval("(update-in my-vec [1] [* 10])");

  // Then
  EXPECT_EQ(result->to_string(), "[1 20 3]");
  EXPECT_EQ(runtime.lookup("my-vec")->to_string(), "[1 2 3]");
}

TEST_F(UpdateInFunction, update_multiple_paths_in_one_call)
{
  // Given
  runtime.eval("(def my-map {:a 1 :nested {:count 2}})");

  // When
  auto result =
    runtime.eval("(update-in my-map [:a] (fn [x] (+ x 1)) [:nested :count] [+ 10])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 2 :nested {:count 12}}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:a 1 :nested {:count 2}}");
}

TEST_F(UpdateInFunction, throws_on_non_sequence_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT([this]() { runtime.eval("(update-in my-map :a (fn [x] x))"); },
              ThrowsMessage<Lisple::TypeError>(
                HasSubstr("Path for update-in must be a sequence")));
}

TEST_F(UpdateInFunction, throws_on_empty_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT([this]() { runtime.eval("(update-in my-map [] (fn [x] x))"); },
              ThrowsMessage<Lisple::InvocationException>(
                HasSubstr("Path for update-in cannot be empty.")));
}

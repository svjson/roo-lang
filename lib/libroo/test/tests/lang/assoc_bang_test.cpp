#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AssocBangFunction = RooTest::RuntimeTestFixture;
TEST_F(AssocBangFunction, add_key_to_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2 :c 3}")->to_string());
}

TEST_F(AssocBangFunction, replace_key_in_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :b 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval("{:a 1 :b 10}")->to_string());
}

TEST_F(AssocBangFunction, replace_key_when_same_value_occurs_before_key)
{
  // Given
  runtime.eval("(def my-map {:id :type :type :type/plain})");

  // When
  auto result = runtime.eval("(assoc! my-map :type :type/updated)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:id :type :type :type/updated}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:id :type :type :type/updated}"));
}

TEST_F(AssocBangFunction, add_and_replace_multiple)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :b 10 :c 3 :d \"some string\")");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}")->to_string());
}

#include "runtime_fixture.h"

#include <gtest/gtest.h>


using AssocInBangFunction = LispleTest::RuntimeTestFixture;
TEST_F(AssocInBangFunction, add_key_to_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in! my-map [:c] 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2 :c 3}")->to_string());
}

TEST_F(AssocInBangFunction, add_key_to_nested_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c {:key1 \"val\"}})");

  // When
  auto result = runtime.eval("(assoc-in! my-map [:c :key2] 44)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c {:key1 \"val\" :key2 44}}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval("{:a 1 :b 2 :c {:key1 \"val\" :key2 44}}")->to_string());
}

TEST_F(AssocInBangFunction, replace_key_in_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc-in! my-map [:b] 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(),
            runtime.eval("{:a 1 :b 10}")->to_string());
}

TEST_F(AssocInBangFunction, replace_key_when_same_value_occurs_before_key)
{
  // Given
  runtime.eval("(def my-map {:id :type :type :type/plain})");

  // When
  auto result = runtime.eval("(assoc-in! my-map [:type] :type/updated)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:id :type :type :type/updated}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:id :type :type :type/updated}"));
}

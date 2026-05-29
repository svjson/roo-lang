#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DissocFunction = LispleTest::RuntimeTestFixture;

TEST_F(DissocFunction, removal_of_non_existing_key_returns_copy)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc my-map :d)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b 2 :c 3}"));
}

TEST_F(DissocFunction, removes_single_key)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc my-map :b)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :c 3}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b 2 :c 3}"));
}

TEST_F(DissocFunction, removes_multiple_keys)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = runtime.eval("(dissoc my-map :a :c)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:b 2 :d 4}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b 2 :c 3 :d 4}"));
}

TEST_F(DissocFunction, ignores_missing_keys_when_removing_multiple_keys)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc my-map :missing :a :other)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:b 2 :c 3}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b 2 :c 3}"));
}

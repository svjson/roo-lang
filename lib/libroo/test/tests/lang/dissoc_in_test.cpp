#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/exception.h>

using DissocInFunction = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(DissocInFunction, removes_key_from_nested_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b {:x 10 :y 20}})");

  // When
  auto result = runtime.eval("(dissoc-in my-map [:b :x])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b {:y 20}}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b {:x 10 :y 20}}"));
}

TEST_F(DissocInFunction, removes_multiple_paths)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b {:x 10 :y 20} :c {:z 30}})");

  // When
  auto result = runtime.eval("(dissoc-in my-map [:a] [:b :y] [:c :z])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:b {:x 10} :c {}}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b {:x 10 :y 20} :c {:z 30}}"));
}

TEST_F(DissocInFunction, traverses_vector_slots)
{
  // Given
  runtime.eval("(def my-map {:items [{:keep 1 :remove 2}]})");

  // When
  auto result = runtime.eval("(dissoc-in my-map [:items 0 :remove])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:items [{:keep 1}]}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:items [{:keep 1 :remove 2}]}"));
}

TEST_F(DissocInFunction, missing_path_does_not_add_intermediate_maps)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When
  auto result = runtime.eval("(dissoc-in my-map [:missing :nested])");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1}"));
}

TEST_F(DissocInFunction, throws_on_non_sequence_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT(
    [this]() { runtime.eval("(dissoc-in my-map :a)"); },
    ThrowsMessage<Roo::TypeError>(HasSubstr("Path for dissoc-in must be a sequence")));
}

TEST_F(DissocInFunction, throws_on_empty_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT([this]() { runtime.eval("(dissoc-in my-map [])"); },
              ThrowsMessage<Roo::InvocationException>(
                HasSubstr("Path for dissoc-in cannot be empty.")));
}

TEST_F(DissocInFunction, throws_when_non_nil_scalar_blocks_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT(
    [this]() { runtime.eval("(dissoc-in my-map [:a :b])"); },
    ThrowsMessage<Roo::TypeError>(HasSubstr("dissoc-in cannot traverse through 1")));
}

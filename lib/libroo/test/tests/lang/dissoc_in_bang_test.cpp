#include <roo/exception.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DissocInBangFunction = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(DissocInBangFunction, removal_of_non_existing_path_returns_nil)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b {:x 10}})");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:b :missing])");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b {:x 10}}"));
}

TEST_F(DissocInBangFunction, removal_of_key_returns_value_and_mutates_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b {:x 10 :y 20}})");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:b :x])");

  // Then
  EXPECT_EQ(*result, *Roo::Value::number(10));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b {:y 20}}"));
}

TEST_F(DissocInBangFunction, traverses_vector_slots)
{
  // Given
  runtime.eval("(def my-map {:items [{:keep 1 :remove 2}]})");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:items 0 :remove])");

  // Then
  EXPECT_EQ(*result, *Roo::Value::number(2));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:items [{:keep 1}]}"));
}

TEST_F(DissocInBangFunction, missing_intermediate_path_returns_nil)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:missing :nested])");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1}"));
}

TEST_F(DissocInBangFunction, throws_on_empty_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT([this]() { runtime.eval("(dissoc-in! my-map [])"); },
              ThrowsMessage<Roo::InvocationException>(
                HasSubstr("Path for dissoc-in! cannot be empty.")));
}

TEST_F(DissocInBangFunction, throws_when_non_nil_scalar_blocks_path)
{
  // Given
  runtime.eval("(def my-map {:a 1})");

  // When/Then
  EXPECT_THAT(
    [this]() { runtime.eval("(dissoc-in! my-map [:a :b])"); },
    ThrowsMessage<Roo::TypeError>(HasSubstr("dissoc-in! cannot remove property from 1")));
}

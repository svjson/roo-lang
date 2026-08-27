#include <roo/exception.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DissocInBangFunction = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(DissocInBangFunction, missing_path_returns_unchanged_target)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b {:x 10}})");
  auto target = runtime.lookup("my-map");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:b :missing])");

  // Then
  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b {:x 10}}"));
}

TEST_F(DissocInBangFunction, removes_key_and_returns_modified_target)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b {:x 10 :y 20}})");
  auto target = runtime.lookup("my-map");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:b :x])");

  // Then
  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b {:y 20}}"));
}

TEST_F(DissocInBangFunction, traverses_vector_slots)
{
  // Given
  runtime.eval("(def my-map {:items [{:keep 1 :remove 2}]})");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:items 0 :remove])");

  // Then
  EXPECT_EQ(result, runtime.lookup("my-map"));
  EXPECT_EQ(*result, *runtime.eval("{:items [{:keep 1}]}"));
}

TEST_F(DissocInBangFunction, missing_intermediate_path_returns_unchanged_target)
{
  // Given
  runtime.eval("(def my-map {:a 1})");
  auto target = runtime.lookup("my-map");

  // When
  auto result = runtime.eval("(dissoc-in! my-map [:missing :nested])");

  // Then
  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:a 1}"));
}

TEST_F(DissocInBangFunction, removes_multiple_paths)
{
  runtime.eval("(def target {:a 1 :b {:x 10 :y 20} :c {:z 30}})");
  auto target = runtime.lookup("target");

  auto result = runtime.eval("(dissoc-in! target [:a] [:b :y] [:c :z])");

  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:b {:x 10} :c {}}"));
}

TEST_F(DissocInBangFunction, accepts_sequential_paths)
{
  runtime.eval("(def target {:nested {:keep 1 :remove 2}})");

  auto result = runtime.eval("(dissoc-in! target '(:nested :remove))");

  EXPECT_EQ(*result, *runtime.eval("{:nested {:keep 1}}"));
}

TEST_F(DissocInBangFunction, removes_multiple_paths_from_nil)
{
  EXPECT_EQ(*runtime.eval("(dissoc-in! nil [:a] [:b :c])"), *Roo::Constant::NIL);
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

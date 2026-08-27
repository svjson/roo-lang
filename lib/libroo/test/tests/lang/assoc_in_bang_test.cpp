#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AssocInBangFunction = RooTest::RuntimeTestFixture;
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

TEST_F(AssocInBangFunction, associates_multiple_paths_and_returns_the_target)
{
  runtime.eval("(def target {:a 1 :nested {:x 2}})");
  auto target = runtime.lookup("target");

  auto result = runtime.eval("(assoc-in! target [:a] 10 [:nested :y] 3 [:b] 20)");

  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:a 10 :nested {:x 2 :y 3} :b 20}"));
}

TEST_F(AssocInBangFunction, creates_missing_maps_below_a_nil_target)
{
  runtime.eval("(def target nil)");

  auto result = runtime.eval("(assoc-in! target [:a :b] 1 [:c] 2)");

  EXPECT_EQ(*result, *runtime.eval("{:a {:b 1} :c 2}"));
  EXPECT_EQ(*runtime.lookup("target"), *Roo::Constant::NIL);
}

TEST_F(AssocInBangFunction, creates_a_missing_intermediate_map)
{
  runtime.eval("(def target {:a 1})");

  auto result = runtime.eval("(assoc-in! target [:nested :value] 2)");

  EXPECT_EQ(result, runtime.lookup("target"));
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :nested {:value 2}}"));
}

TEST_F(AssocInBangFunction, accepts_sequential_paths)
{
  runtime.eval("(def target {:nested {}})");

  auto result = runtime.eval("(assoc-in! target '(:nested :value) 2)");

  EXPECT_EQ(*result, *runtime.eval("{:nested {:value 2}}"));
}

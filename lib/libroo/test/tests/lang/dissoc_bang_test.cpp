
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DissocBangFunction = RooTest::RuntimeTestFixture;
TEST_F(DissocBangFunction, removal_of_non_existing_key_returns_unchanged_target)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");
  auto target = runtime.lookup("my-map");

  // When
  auto result = runtime.eval("(dissoc! my-map :d)");

  // Then
  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
}

TEST_F(DissocBangFunction, removes_key_and_returns_modified_target)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");
  auto target = runtime.lookup("my-map");

  // When
  auto result = runtime.eval("(dissoc! my-map :b)");

  // Then
  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :c 3}"));
}

TEST_F(DissocBangFunction, removes_multiple_keys_and_returns_modified_target)
{
  runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");
  auto target = runtime.lookup("my-map");

  auto result = runtime.eval("(dissoc! my-map :a :c)");

  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:b 2 :d 4}"));
}

TEST_F(DissocBangFunction, ignores_missing_keys_when_removing_multiple_keys)
{
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  auto result = runtime.eval("(dissoc! my-map :missing :a :other)");

  EXPECT_EQ(result, runtime.lookup("my-map"));
  EXPECT_EQ(*result, *runtime.eval("{:b 2 :c 3}"));
}

TEST_F(DissocBangFunction, removes_multiple_keys_from_nil)
{
  EXPECT_EQ(*runtime.eval("(dissoc! nil :a :b)"), *Roo::Constant::NIL);
}

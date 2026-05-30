
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DissocBangFunction = RooTest::RuntimeTestFixture;
TEST_F(DissocBangFunction, removal_of_non_existing_key_returns_nil)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc! my-map :d)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::NIL);
  EXPECT_EQ(runtime.eval("my-map")->to_string(), "{:a 1 :b 2 :c 3}");
}

TEST_F(DissocBangFunction, removal_of_key_returns_value)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc! my-map :b)");

  // Then
  EXPECT_EQ(*result, *Roo::Value::number(2));
  EXPECT_EQ(runtime.eval("my-map")->to_string(), "{:a 1 :c 3}");
}

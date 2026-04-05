
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(DissocBangFunction, removal_of_non_existing_key_returns_nil)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc! my-map :d)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::NIL);
  EXPECT_EQ(runtime.eval("my-map")->to_string(), "{:a 1 :b 2 :c 3}");
}

TEST(DissocBangFunction, removal_of_key_returns_value)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc! my-map :b)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(2));
  EXPECT_EQ(runtime.eval("my-map")->to_string(), "{:a 1 :c 3}");
}

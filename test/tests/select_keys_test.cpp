
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(SelectKeysFunction, all_keys_present)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = runtime.eval("(select-keys my-map [:b :d])");

  // Then
  ASSERT_EQ(result->to_string(), "{:b 2 :d 4}");
}

TEST(SelectKeysFunction, no_keys_present)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = runtime.eval("(select-keys my-map [:f :g])");

  // Then
  ASSERT_EQ(result->to_string(), "{}");
}

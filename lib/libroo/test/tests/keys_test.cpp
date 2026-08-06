
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using KeysFunction = RooTest::RuntimeTestFixture;

TEST_F(KeysFunction, map_with_keys)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = runtime.eval("(keys my-map)");

  // Then
  ASSERT_EQ(result->to_string(), "[:a :b :c :d]");
}

TEST_F(KeysFunction, empty_map)
{
  // When
  auto result = runtime.eval("(keys {})");

  // Then
  ASSERT_EQ(result->to_string(), "[]");
}

TEST_F(KeysFunction, keys_of_nil)
{
  // When
  auto result = runtime.eval("(keys nil)");

  // Then
  ASSERT_EQ(result->to_string(), "[]");
}

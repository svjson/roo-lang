
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ValsFunction = RooTest::RuntimeTestFixture;

TEST_F(ValsFunction, map_with_keys)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = runtime.eval("(vals my-map)");

  // Then
  ASSERT_EQ(result->to_string(), "[1 2 3 4]");
}

TEST_F(ValsFunction, empty_map)
{
  // When
  auto result = runtime.eval("(vals {})");

  // Then
  ASSERT_EQ(result->to_string(), "[]");
}

TEST_F(ValsFunction, vals_of_nil)
{
  // When
  auto result = runtime.eval("(vals nil)");

  // Then
  ASSERT_EQ(result->to_string(), "[]");
}

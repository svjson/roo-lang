
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using GetFunction = RooTest::RuntimeTestFixture;
TEST_F(GetFunction, get_from_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get my-map :b)");

  // Then
  ASSERT_EQ(result->i64(), 2);
}

TEST_F(GetFunction, get_dynamic_value_from_map)
{
  // Given
  runtime.eval("(def val {:type-1 :a :type-2 :b})");
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result1 = runtime.eval("(get my-map (:type-1 val))");
  auto result2 = runtime.eval("(get my-map (:type-2 val))");

  // Then
  ASSERT_EQ(result1->i32(), 1);
  ASSERT_EQ(result2->i32(), 2);
}

TEST_F(GetFunction, get_dynamic_value_from_map_by_qualified_key)
{
  // Given
  runtime.eval("(def val {:type-1 :type/a :type-2 :type/b})");
  runtime.eval("(def my-map {:type/a 1 :type/b 2})");

  // When
  auto result1 = runtime.eval("(get my-map (:type-1 val))");
  auto result2 = runtime.eval("(get my-map (:type-2 val))");

  // Then
  ASSERT_EQ(result1->i32(), 1);
  ASSERT_EQ(result2->i32(), 2);
}

TEST_F(GetFunction, get_returns_default_for_missing_key)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval(R"((get my-map :c "default-value"))");

  // Then
  ASSERT_EQ(*result, *Roo::Value::string("default-value"));
}

TEST_F(GetFunction, get_does_not_return_default_for_present_nil)
{
  // Given
  runtime.eval("(def my-map {:key nil})");

  // When
  auto result = runtime.eval(R"((get my-map :key "default-value"))");

  // Then
  ASSERT_EQ(*result, *Roo::Constant::NIL);
}

TEST_F(GetFunction, get_does_not_return_default_for_present_vector_nil)
{
  // Given
  runtime.eval("(def my-vec [nil])");

  // When
  auto result = runtime.eval(R"((get my-vec 0 "default-value"))");

  // Then
  ASSERT_EQ(*result, *Roo::Constant::NIL);
}

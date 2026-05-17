
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using GetInFunction = LispleTest::RuntimeTestFixture;
TEST_F(GetInFunction, get_single_key_path_from_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get-in my-map [:b])");

  // Then
  ASSERT_EQ(result->i32(), 2);
}

TEST_F(GetInFunction, get_non_existing_multi_key_path_from_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get-in my-map [:c :d :e])");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST_F(GetInFunction, get_non_existing_multi_key_path_through_number_from_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get-in my-map [:a :d :e])");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST_F(GetInFunction, get_existing_multi_key_path_from_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b {:c {:d 2}}})");

  // When
  auto result = runtime.eval("(get-in my-map [:b :c :d])");

  // Then
  ASSERT_EQ(result->i32(), 2);
}

TEST_F(GetInFunction, get_nested_vector_element)
{
  // Given
  runtime.eval("(def my-vec [[1 2 3] [4 5 6]])");

  // When
  auto result = runtime.eval("(get-in my-vec [1 2])");

  // Then
  ASSERT_EQ(result->to_string(), "6");
}

TEST_F(GetInFunction, get_in_returns_default_for_missing_path)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval(R"((get-in my-map [:c :d :e] "default-value"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Value::string("default-value"));
}

TEST_F(GetInFunction, get_in_does_not_return_default_for_present_nil)
{
  // Given
  runtime.eval("(def my-map {:a {:b nil}})");

  // When
  auto result = runtime.eval(R"((get-in my-map [:a :b] "default-value"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST_F(GetInFunction, get_in_returns_default_when_remaining_path_is_missing_after_nil)
{
  // Given
  runtime.eval("(def my-map {:a nil})");

  // When
  auto result = runtime.eval(R"((get-in my-map [:a :b] "default-value"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Value::string("default-value"));
}


#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(GetInFunction, get_single_key_path_from_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get-in my-map [:b])");

  // Then
  ASSERT_EQ(result->i32(), 2);
}

TEST(GetInFunction, get_non_existing_multi_key_path_from_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get-in my-map [:c :d :e])");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST(GetInFunction, get_non_existing_multi_key_path_through_number_from_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(get-in my-map [:a :d :e])");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST(GetInFunction, get_existing_multi_key_path_from_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b {:c {:d 2}}})");

  // When
  auto result = runtime.eval("(get-in my-map [:b :c :d])");

  // Then
  ASSERT_EQ(result->i32(), 2);
}

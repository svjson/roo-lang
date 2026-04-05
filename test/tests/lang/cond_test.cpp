
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(CondForm, match_condition)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def x 20)");

  // When
  auto result = runtime.eval(
    R"((cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zwanzig"));
}

TEST(CondForm, no_match_with_else)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def x 100)");

  // When
  auto result = runtime.eval(
    R"((cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions"))");
  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zillions"));
}

TEST(CondForm, no_match_without_else)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def x 100)");

  // When
  auto result = runtime.eval(R"((cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

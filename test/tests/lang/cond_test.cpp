
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(CondMacro, match_condition)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval(
    R"((let [x 20] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions")))");

  // Then
  ASSERT_EQ(*result, Lisple::String("Zwanzig"));
}

TEST(CondMacro, no_match_with_else)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval(
    R"((let [x 100] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions")))");
  // Then
  ASSERT_EQ(*result, Lisple::String("Zillions"));
}

TEST(CondMacro, no_match_without_else)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval(
    R"((let [x 100] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig")))");
  // Then
  ASSERT_EQ(*result, *Lisple::NIL);
}

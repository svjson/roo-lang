
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CondForm = LispleTest::RuntimeTestFixture;
TEST_F(CondForm, match_condition)
{
  // Given
  runtime.eval("(def x 20)");

  // When
  auto result = runtime.eval(
    R"((cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Value::string("Zwanzig"));
}

TEST_F(CondForm, no_match_with_else)
{
  // Given
  runtime.eval("(def x 100)");

  // When
  auto result = runtime.eval(
    R"((cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions"))");
  // Then
  ASSERT_EQ(*result, *Lisple::Value::string("Zillions"));
}

TEST_F(CondForm, no_match_without_else)
{
  // Given
  runtime.eval("(def x 100)");

  // When
  auto result = runtime.eval(R"((cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

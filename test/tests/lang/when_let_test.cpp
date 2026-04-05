#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(WhenLetForm, when_let)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a 10})] value)")->to_string(), "10");
  EXPECT_EQ(runtime.eval("(when-let [value (:b {:a 10})] value)")->to_string(), "nil");
}

TEST(WhenLetForm, condition_check_must_happen_only_at_current_scope_level)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def value 1234)");

  // When
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a 10})] value)")->to_string(), "10");
  EXPECT_EQ(runtime.eval("(when-let [value (:b {:a 10})] value)")->to_string(), "nil");
}

TEST(WhenLetForm, branching_should_happen_according_to_truthiness_not_just_ifdef)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a true})] value)")->to_string(), "true");
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a false})] value)")->to_string(), "nil");
}

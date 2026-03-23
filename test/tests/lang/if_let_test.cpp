#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(IfLetForm, if_let)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a 10})] value \"no value\")")->to_string(),
            "10");
  EXPECT_EQ(runtime.eval("(if-let [value (:b {:a 10})] value \"no value\")")->to_string(),
            "\"no value\"");
}

TEST(IfLetForm, if_check_must_happen_only_at_current_scope_level)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def value 1234)");

  // When
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a 10})] value \"no value\")")->to_string(),
            "10");
  EXPECT_EQ(runtime.eval("(if-let [value (:b {:a 10})] value \"no value\")")->to_string(),
            "\"no value\"");
}

TEST(IfLetForm, branching_should_happen_according_to_truthiness_not_just_ifdef)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a true})] value \"no value\")")->to_string(),
            "true");
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a false})] value \"no value\")")->to_string(),
            "\"no value\"");
}

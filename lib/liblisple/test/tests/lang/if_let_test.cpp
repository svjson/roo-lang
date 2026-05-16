#include <gtest/gtest.h>
#include "runtime_fixture.h"


using IfLetForm = LispleTest::RuntimeTestFixture;
TEST_F(IfLetForm, if_let)
{
  // Given
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a 10})] value \"no value\")")->to_string(),
            "10");
  EXPECT_EQ(runtime.eval("(if-let [value (:b {:a 10})] value \"no value\")")->to_string(),
            "\"no value\"");
}

TEST_F(IfLetForm, if_check_must_happen_only_at_current_scope_level)
{
  // Given
  runtime.eval("(def value 1234)");

  // When
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a 10})] value \"no value\")")->to_string(),
            "10");
  EXPECT_EQ(runtime.eval("(if-let [value (:b {:a 10})] value \"no value\")")->to_string(),
            "\"no value\"");
}

TEST_F(IfLetForm, branching_should_happen_according_to_truthiness_not_just_ifdef)
{
  // Given
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a true})] value \"no value\")")->to_string(),
            "true");
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a false})] value \"no value\")")->to_string(),
            "\"no value\"");
}

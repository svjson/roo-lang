#include "runtime_fixture.h"
#include <gtest/gtest.h>

using WhenLetForm = RooTest::RuntimeTestFixture;
TEST_F(WhenLetForm, when_let)
{
  // Given
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a 10})] value)")->to_string(), "10");
  EXPECT_EQ(runtime.eval("(when-let [value (:b {:a 10})] value)")->to_string(), "nil");
}

TEST_F(WhenLetForm, condition_check_must_happen_only_at_current_scope_level)
{
  // Given
  runtime.eval("(def value 1234)");

  // When
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a 10})] value)")->to_string(), "10");
  EXPECT_EQ(runtime.eval("(when-let [value (:b {:a 10})] value)")->to_string(), "nil");
}

TEST_F(WhenLetForm, branching_should_happen_according_to_truthiness_not_just_ifdef)
{
  // Given
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a true})] value)")->to_string(), "true");
  EXPECT_EQ(runtime.eval("(when-let [value (:a {:a false})] value)")->to_string(), "nil");
}

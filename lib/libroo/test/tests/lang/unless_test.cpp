#include "runtime_fixture.h"
#include <gtest/gtest.h>

using UnlessForm = RooTest::RuntimeTestFixture;

TEST_F(UnlessForm, evaluates_body_when_condition_is_falsy)
{
  // Then
  EXPECT_EQ(runtime.eval("(unless false 42)")->to_string(), "42");
  EXPECT_EQ(runtime.eval("(unless nil 42)")->to_string(), "42");
}

TEST_F(UnlessForm, returns_nil_when_condition_is_truthy)
{
  // Then
  EXPECT_EQ(runtime.eval("(unless true 42)")->to_string(), "nil");
  EXPECT_EQ(runtime.eval("(unless 1 42)")->to_string(), "nil");
}

TEST_F(UnlessForm, evaluates_multiple_body_forms_and_returns_last)
{
  // Then
  EXPECT_EQ(runtime.eval("(unless false 1 2 3)")->to_string(), "3");
}

TEST_F(UnlessForm, returns_nil_with_no_body_forms_evaluated)
{
  // Then
  EXPECT_EQ(runtime.eval("(unless true)")->to_string(), "nil");
  EXPECT_EQ(runtime.eval("(unless false)")->to_string(), "nil");
}

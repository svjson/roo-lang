
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DoTimesForm = RooTest::RuntimeTestFixture;
TEST_F(DoTimesForm, symbol_bound__explicit_iterations)
{
  // Given
  auto result = runtime.eval("(dotimes [n 10] (* n 2))");

  // Then
  EXPECT_EQ(result->to_string(), "[0 2 4 6 8 10 12 14 16 18]");
}

TEST_F(DoTimesForm, symbol_bound__iterations_as_expression)
{
  // Given
  auto result = runtime.eval("(dotimes [n (- 10 5)] (* n 2))");

  // Then
  EXPECT_EQ(result->to_string(), "[0 2 4 6 8]");
}

TEST_F(DoTimesForm, no_binding__explicit_iterations)
{
  // Given
  auto result = runtime.eval("(dotimes [5] :repeat-me)");

  // Then
  EXPECT_EQ(result->to_string(), "[:repeat-me :repeat-me :repeat-me :repeat-me :repeat-me]");
}

TEST_F(DoTimesForm, no_binding__iterations_as_expression)
{
  // Given
  auto result = runtime.eval("(dotimes [(- 5 2)] :repeat-me)");

  // Then
  EXPECT_EQ(result->to_string(), "[:repeat-me :repeat-me :repeat-me]");
}

TEST_F(DoTimesForm, empty_body_returns_nil_values)
{
  auto result = runtime.eval("(dotimes [n 3])");

  ASSERT_TRUE(result.get());
  ASSERT_EQ(result->to_string(), "[nil nil nil]");
  for (auto& value : result->elements())
  {
    ASSERT_TRUE(value.get());
    ASSERT_EQ(value, Roo::Constant::NIL);
  }
}

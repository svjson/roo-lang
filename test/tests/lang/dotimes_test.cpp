
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(DoTimesForm, symbol_bound__explicit_iterations)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(dotimes [n 10] (* n 2))");

  // Then
  EXPECT_EQ(result->to_string(), "[0 2 4 6 8 10 12 14 16 18]");
}

TEST(DoTimesForm, symbol_bound__iterations_as_expression)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(dotimes [n (- 10 5)] (* n 2))");

  // Then
  EXPECT_EQ(result->to_string(), "[0 2 4 6 8]");
}

TEST(DoTimesForm, no_binding__explicit_iterations)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(dotimes [5] :repeat-me)");

  // Then
  EXPECT_EQ(result->to_string(), "[:repeat-me :repeat-me :repeat-me :repeat-me :repeat-me]");
}

TEST(DoTimesForm, no_binding__iterations_as_expression)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(dotimes [(- 5 2)] :repeat-me)");

  // Then
  EXPECT_EQ(result->to_string(), "[:repeat-me :repeat-me :repeat-me]");
}

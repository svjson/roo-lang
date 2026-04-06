
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(EvalFunction, eval_string)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(+ 10 100)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(110));
}

TEST(EvalFunction, eval_list)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(eval '(+ 8 90))");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(98));
}

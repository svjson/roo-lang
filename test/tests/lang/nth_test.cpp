
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(NthFunction, nth_valid_numbers)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(nth [0 1 2 3 4] 0)"), *Lisple::Number::make(0));
  EXPECT_EQ(*runtime.eval("(nth [0 1 2 3 4] 1)"), *Lisple::Number::make(1));
  EXPECT_EQ(*runtime.eval("(nth [0 1 2 3 4] 2)"), *Lisple::Number::make(2));
  EXPECT_EQ(*runtime.eval("(nth [0 1 2 3 4] 3)"), *Lisple::Number::make(3));
  EXPECT_EQ(*runtime.eval("(nth [0 1 2 3 4] 4)"), *Lisple::Number::make(4));
}

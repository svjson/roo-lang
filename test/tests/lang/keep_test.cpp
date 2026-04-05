#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ======================================================================
 * KeepFunction - (keep [...] select-transform-fm)
 * ======================================================================
 */

TEST(KeepFunction, transform_even)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result =
    runtime.eval("(keep [1 2 3 4] (fn [x] (when (even? x) (str \"Number \" x))))");

  // Then
  ASSERT_EQ(*result,
            *Lisple::RTValue::vector(
              {Lisple::RTValue::string("Number 2"), Lisple::RTValue::string("Number 4")}));
}

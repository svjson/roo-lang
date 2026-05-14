#include "runtime_fixture.h"
#include <gtest/gtest.h>

using KeepFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * KeepFunction - (keep [...] select-transform-fm)
 * ======================================================================
 */

TEST_F(KeepFunction, transform_even)
{
  // Given
  auto result =
    runtime.eval("(keep [1 2 3 4] (fn [x] (when (even? x) (str \"Number \" x))))");

  // Then
  ASSERT_EQ(*result,
            *Lisple::Value::vector(
              {Lisple::Value::string("Number 2"), Lisple::Value::string("Number 4")}));
}

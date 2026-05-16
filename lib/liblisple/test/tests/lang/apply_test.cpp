
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using ApplyFunction = LispleTest::RuntimeTestFixture;
TEST_F(ApplyFunction, apply_concat)
{
  // Given
  auto retval = runtime.eval("(apply concat [[1 2 3] [4 5 6]])");

  // Then
  ASSERT_EQ(retval->to_string(), "[1 2 3 4 5 6]");
}

TEST_F(ApplyFunction, apply_dynamic)
{
  // Given
  runtime.eval("(def function-name 'max)");
  auto result = runtime.eval("(apply (resolve function-name) [10 8 17 4 0])");

  // Then
  ASSERT_EQ(result->to_string(), "17");
}

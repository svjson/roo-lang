
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using EvalFunction = LispleTest::RuntimeTestFixture;
TEST_F(EvalFunction, eval_string)
{
  // Given
  auto result = runtime.eval("(+ 10 100)");

  // Then
  EXPECT_EQ(*result, *Lisple::Value::number(110));
}

TEST_F(EvalFunction, eval_list)
{
  // Given
  auto result = runtime.eval("(eval '(+ 8 90))");

  // Then
  EXPECT_EQ(*result, *Lisple::Value::number(98));
}

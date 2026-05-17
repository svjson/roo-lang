#include "runtime_fixture.h"

#include <exception>

#include <gtest/gtest.h>

using Semantic_ExceptionRestoration = LispleTest::RuntimeTestFixture;

TEST_F(Semantic_ExceptionRestoration,
       user_function_exception_restores_caller_namespace_and_scope)
{
  runtime.eval(R"(
    (ns caller)
    (def caller-marker :caller)
  )");

  runtime.eval(R"(
    (ns callee)
    (defun broken [leaked-local]
      (+ "bad" leaked-local))
  )");

  runtime.switch_namespace("caller");

  EXPECT_THROW(runtime.eval("(callee/broken 10)"), std::exception);

  EXPECT_EQ(runtime.get_current_namespace().get_name(), "caller");
  EXPECT_EQ(runtime.eval("caller-marker")->to_string(), ":caller");
  EXPECT_THROW(runtime.eval("leaked-local"), std::exception);
}

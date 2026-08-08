#include "runtime_fixture.h"
#include <gtest/gtest.h>

using LexicalScope = RooTest::RuntimeTestFixture;

TEST_F(LexicalScope, defun_call_cannot_resolve_caller_let_binding)
{
  EXPECT_THROW(runtime.eval(R"(
                 (defun overwrite-outer [] (set! [x] 999))
                 (defun outer [] (let [x 1] (overwrite-outer) x))
                 (outer)
               )"),
               std::exception);
}

TEST_F(LexicalScope, lambda_does_not_capture_unrelated_caller_binding)
{
  runtime.eval(R"(
    (defun make-adder-wrongly-scoped []
      (fn [x] (+ x secret)))
    (defun caller-with-secret []
      (let [secret 1000] (make-adder-wrongly-scoped)))
    (def adder (caller-with-secret))
  )");

  EXPECT_THROW(runtime.eval("(adder 1)"), std::exception);
}

TEST_F(LexicalScope, lambda_set_mutates_lexically_captured_binding)
{
  EXPECT_THROW(runtime.eval(R"(
                 (defun make-mutator [] (fn [] (set! [counter] 42)))
                 (defun run-it []
                   (let [counter 0]
                     (let [mutator (make-mutator)] (mutator) counter)))
                 (run-it)
               )"),
               std::exception);
}

TEST_F(LexicalScope, lambda_retains_mutated_enclosing_binding_across_calls)
{
  auto result = runtime.eval(R"(
    (def counter
      (let [value 0]
        (fn []
          (set! [value] (+ value 1))
          value)))
    [(counter) (counter)]
  )");

  EXPECT_EQ(*result, *runtime.eval("[1 2]"));
}

TEST_F(LexicalScope, separately_created_lambdas_have_independent_bindings)
{
  auto result = runtime.eval(R"(
    (defun make-counter []
      (let [value 0]
        (fn []
          (set! [value] (+ value 1))
          value)))
    (def first-counter (make-counter))
    (def second-counter (make-counter))
    [(first-counter) (first-counter) (second-counter)]
  )");

  EXPECT_EQ(*result, *runtime.eval("[1 2 1]"));
}


#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CondLetForm = RooTest::RuntimeTestFixture;

TEST_F(CondLetForm, returns_nil_with_no_clauses)
{
  // When
  auto result = runtime.eval("(cond-let)");

  // Then
  ASSERT_EQ(result->type, Roo::Value::Type::NIL);
}

TEST_F(CondLetForm, returns_nil_when_no_clause_matches)
{
  // When
  auto result = runtime.eval("(cond-let false 1 nil 2)");

  // Then
  ASSERT_EQ(result->type, Roo::Value::Type::NIL);
}

TEST_F(CondLetForm, plain_truthy_clause_taken)
{
  // When
  auto result = runtime.eval("(cond-let false 1 true 42)");

  // Then
  ASSERT_EQ(result->i64(), 42);
}

TEST_F(CondLetForm, binding_clause_binds_value_in_body)
{
  // When
  auto result = runtime.eval("(cond-let [x 10] (* x 2))");

  // Then
  ASSERT_EQ(result->i64(), 20);
}

TEST_F(CondLetForm, falsy_binding_clause_skipped)
{
  // When -- nil is falsy, so first clause is skipped
  auto result = runtime.eval("(cond-let [x nil] :never true :found)");

  // Then
  ASSERT_EQ(result->type, Roo::Value::Type::KEYWORD);
  ASSERT_EQ(result->str(), "found");
}

TEST_F(CondLetForm, false_binding_clause_skipped)
{
  // When
  auto result = runtime.eval("(cond-let [x false] :never true 99)");

  // Then
  ASSERT_EQ(result->i64(), 99);
}

TEST_F(CondLetForm, binding_not_visible_outside_its_clause)
{
  // When -- cond-let introduces x=99, but outer let has x=:outer
  auto result = runtime.eval(
    "(let [x :outer]"
    "  (cond-let [x 99] x)"
    "  x)");

  // Then -- outer x is unchanged after the clause's scope ends
  ASSERT_EQ(result->str(), "outer");
}

TEST_F(CondLetForm, first_truthy_clause_wins)
{
  // When -- only the first truthy clause should be evaluated
  auto result = runtime.eval("(cond-let [x 5] x [y 99] y)");

  // Then
  ASSERT_EQ(result->i64(), 5);
}

TEST_F(CondLetForm, second_binding_clause_taken_when_first_falsy)
{
  // When
  auto result = runtime.eval("(cond-let [x nil] x [y 7] (* y 3))");

  // Then
  ASSERT_EQ(result->i64(), 21);
}

TEST_F(CondLetForm, mixed_plain_and_binding_clauses)
{
  // When
  auto result = runtime.eval(
    "(cond-let"
    "  false          :no"
    "  [x nil]        x"
    "  [found 42]     (+ found 1)"
    "  true           :too-late)");

  // Then
  ASSERT_EQ(result->i64(), 43);
}

TEST_F(CondLetForm, else_as_plain_truthy_fallback)
{
  // When -- :else is a truthy keyword, acts as plain cond fallback
  auto result = runtime.eval("(cond-let [x nil] x :else :default)");

  // Then
  ASSERT_EQ(result->str(), "default");
}

TEST_F(CondLetForm, binding_clause_body_not_evaluated_when_falsy)
{
  // Given
  runtime.eval("(def call-count 0)");
  runtime.eval("(defun side-effect [] (set! [call-count] (inc call-count)) :result)");

  // When -- nil binding is falsy so body must not run
  runtime.eval("(cond-let [x nil] (side-effect) true :done)");

  // Then
  ASSERT_EQ(runtime.eval("call-count")->i64(), 0);
}

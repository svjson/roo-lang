
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CondThreadFirst = RooTest::RuntimeTestFixture;

TEST_F(CondThreadFirst, returns_nil_with_no_args)
{
  // When
  auto result = runtime.eval("(cond->)");

  // Then
  ASSERT_EQ(result->type, Roo::Value::Type::NIL);
}

TEST_F(CondThreadFirst, returns_value_unchanged_when_no_pairs)
{
  // When
  auto result = runtime.eval("(cond-> 42)");

  // Then
  ASSERT_EQ(result->i64(), 42);
}

TEST_F(CondThreadFirst, passes_through_when_condition_is_false)
{
  // When
  auto result = runtime.eval("(cond-> 5 false (+ 10))");

  // Then
  ASSERT_EQ(result->i64(), 5);
}

TEST_F(CondThreadFirst, passes_through_when_condition_is_nil)
{
  // When
  auto result = runtime.eval("(cond-> 5 nil (+ 10))");

  // Then
  ASSERT_EQ(result->i64(), 5);
}

TEST_F(CondThreadFirst, applies_step_when_condition_is_true)
{
  // When
  auto result = runtime.eval("(cond-> 5 true (+ 10))");

  // Then
  ASSERT_EQ(result->i64(), 15);
}

TEST_F(CondThreadFirst, applies_all_true_steps_in_sequence)
{
  // When  -- (1 + 10) * 3 = 33
  auto result = runtime.eval("(cond-> 1 true (+ 10) true (* 3))");

  // Then
  ASSERT_EQ(result->i64(), 33);
}

TEST_F(CondThreadFirst, skips_false_steps_applies_true_ones)
{
  // When  -- (1 + 10) + 5 = 16, the (* 3) step is skipped
  auto result = runtime.eval("(cond-> 1 true (+ 10) false (* 3) true (+ 5))");

  // Then
  ASSERT_EQ(result->i64(), 16);
}

TEST_F(CondThreadFirst, bare_symbol_form_receives_threaded_value)
{
  // When  -- inc is a bare symbol (not a list), threaded value is its only arg
  auto result = runtime.eval("(cond-> 5 true inc)");

  // Then
  ASSERT_EQ(result->i64(), 6);
}

TEST_F(CondThreadFirst, form_with_multiple_extra_args)
{
  // Given  -- assoc with 2 kv pairs has extra_arg_count = 3, not 1
  runtime.eval("(def result (cond-> {} true (assoc :a 1 :b 2)))");

  // Then
  EXPECT_EQ(*runtime.eval("(:a result)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(:b result)"), *Roo::Value::number(2));
}

TEST_F(CondThreadFirst, threads_into_map_operations)
{
  // Given
  runtime.eval("(def base {:count 0})");

  // When
  runtime.eval("(def result (cond-> base true (assoc :touched? true) false (assoc :count 99)))");

  // Then
  EXPECT_EQ(*runtime.eval("(:touched? result)"), *Roo::Value::boolean(true));
  EXPECT_EQ(*runtime.eval("(:count result)"),    *Roo::Value::number(0));
}

TEST_F(CondThreadFirst, condition_uses_lexical_scope_not_threaded_value)
{
  // Given
  runtime.eval("(def flag true)");

  // When  -- flag is resolved from the outer scope, not the threaded value
  auto result = runtime.eval("(cond-> 10 flag (+ 5))");

  // Then
  ASSERT_EQ(result->i64(), 15);
}

TEST_F(CondThreadFirst, false_branch_form_is_not_called)
{
  // Given
  runtime.eval("(def call-count 0)");
  runtime.eval("(defun counting-id [v] (set! [call-count] (inc call-count)) v)");

  // When
  runtime.eval("(cond-> 5 false counting-id)");

  // Then
  ASSERT_EQ(runtime.eval("call-count")->i64(), 0);
}

TEST_F(CondThreadFirst, false_branch_args_are_not_evaluated)
{
  // Given
  runtime.eval("(def eval-count 0)");
  runtime.eval("(defun counted [v] (set! [eval-count] (inc eval-count)) v)");

  // When  -- the arg (counted 10) is inside a false branch and must not be evaluated
  runtime.eval("(cond-> 5 false (+ (counted 10)))");

  // Then
  ASSERT_EQ(runtime.eval("eval-count")->i64(), 0);
}

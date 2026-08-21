#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CallAtForm = RooTest::RuntimeTestFixture;

TEST_F(CallAtForm, invokes_callable_from_target)
{
  runtime.eval("(def handlers {:sum (fn [a b] (+ a b))})");

  EXPECT_EQ(runtime.eval("(@> handlers (sum 2 3))")->to_string(), "5");
}

TEST_F(CallAtForm, falls_back_to_symbol_key)
{
  runtime.eval("(def handlers {'sum (fn [a b] (+ a b))})");

  EXPECT_EQ(runtime.eval("(@> handlers (sum 2 3))")->to_string(), "5");
}

TEST_F(CallAtForm, bare_symbol_prefers_keyword_key)
{
  runtime.eval("(def handlers {:selected (fn [] :keyword) 'selected (fn [] :symbol)})");

  EXPECT_EQ(*runtime.eval("(@> handlers (selected))"), *Roo::Value::keyword("keyword"));
}

TEST_F(CallAtForm, explicit_key_type_resolves_collisions)
{
  runtime.eval("(def handlers {:selected (fn [] :keyword) 'selected (fn [] :symbol)})");

  EXPECT_EQ(*runtime.eval("(@> handlers (:selected))"), *Roo::Value::keyword("keyword"));
  EXPECT_EQ(*runtime.eval("(@> handlers ('selected))"), *Roo::Value::keyword("symbol"));
}

TEST_F(CallAtForm, explicit_key_type_does_not_fall_back)
{
  runtime.eval("(def symbol-handlers {'selected (fn [] :symbol)})");
  runtime.eval("(def keyword-handlers {:selected (fn [] :keyword)})");

  EXPECT_THROW(runtime.eval("(@> symbol-handlers (:selected))"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(@> keyword-handlers ('selected))"), Roo::InvocationException);
}

TEST_F(CallAtForm, present_keyword_value_prevents_symbol_fallback)
{
  runtime.eval("(def handlers {:selected nil 'selected (fn [] :symbol)})");

  EXPECT_THROW(runtime.eval("(@> handlers (selected))"), Roo::InvocationException);
}

TEST_F(CallAtForm, invokes_callable_from_module_view)
{
  runtime.eval("(ns my-app.actions)");
  runtime.eval("(defun execute! [value] (+ value 1))");
  runtime.eval("(ns caller)");
  runtime.eval("(def actions (module 'my-app.actions))");

  EXPECT_EQ(*runtime.eval("(@> actions (execute! 41))"), *Roo::Value::number(42));
}

TEST_F(CallAtForm, invokes_callable_below_target_path)
{
  runtime.eval("(def application {:commands {:sum (fn [a b] (+ a b))}})");

  EXPECT_EQ(runtime.eval("(@> [application :commands] (sum 2 3))")->to_string(), "5");
}

TEST_F(CallAtForm, target_path_uses_get_in_key_semantics)
{
  runtime.eval(R"(
    (def command-key "commands")
    (def application {"commands" {:answer (fn [] 42)}})
  )");

  EXPECT_EQ(runtime.eval("(@> [application command-key] (answer))")->to_string(), "42");
}

TEST_F(CallAtForm, target_path_preserves_symbol_keys)
{
  runtime.eval("(def application {'commands {:answer (fn [] 42)}})");

  EXPECT_EQ(runtime.eval("(@> [application 'commands] (answer))")->to_string(), "42");
}

TEST_F(CallAtForm, single_element_target_path_is_valid)
{
  runtime.eval("(def handlers {:answer (fn [] 42)})");

  EXPECT_EQ(runtime.eval("(@> [handlers] (answer))")->to_string(), "42");
}

TEST_F(CallAtForm, call_head_shadows_outer_binding_throughout_call)
{
  runtime.eval("(def handlers {:selected (fn [value] (fn? value))})");

  EXPECT_EQ(
    runtime.eval("(let [selected :outer] (@> handlers (selected selected)))")->to_string(),
    "true");
}

TEST_F(CallAtForm, target_and_path_are_evaluated_once)
{
  runtime.eval(R"(
    (def target-calls 0)
    (def path-calls 0)
    (def application {:commands {:answer (fn [] 42)}})
    (defun target! [] (set! [target-calls] (inc target-calls)) application)
    (defun path! [] (set! [path-calls] (inc path-calls)) :commands)
  )");

  EXPECT_EQ(runtime.eval("(@> [(target!) (path!)] (answer))")->to_string(), "42");
  EXPECT_EQ(runtime.eval("target-calls")->to_string(), "1");
  EXPECT_EQ(runtime.eval("path-calls")->to_string(), "1");
}

TEST_F(CallAtForm, missing_callable_uses_normal_callable_error)
{
  EXPECT_THROW(runtime.eval("(@> {} (missing))"), Roo::InvocationException);
}

TEST_F(CallAtForm, restores_outer_scope_after_callable_throws)
{
  runtime.eval(R"(
    (def selected :outer)
    (def handlers {:selected (fn [] (+ "bad" 1))})
  )");

  EXPECT_THROW(runtime.eval("(@> handlers (selected))"), Roo::InvocationException);
  EXPECT_EQ(runtime.eval("selected")->to_string(), ":outer");
}

TEST_F(CallAtForm, rejects_empty_target_path)
{
  EXPECT_THROW(runtime.eval("(@> [] (missing))"), Roo::TypeError);
}

TEST_F(CallAtForm, rejects_non_call_second_argument)
{
  EXPECT_THROW(runtime.eval("(@> {} :missing)"), Roo::TypeError);
}

TEST_F(CallAtForm, rejects_invalid_call_head)
{
  EXPECT_THROW(runtime.eval("(@> {} (42))"), Roo::TypeError);
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_FunctionArgumentShadowing = RooTest::RuntimeTestFixture;

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_same_named_function_in_argument_position)
{
  runtime.eval(R"(
  (defun selected-tab-id [tabs selected-tab]
    (:id selected-tab))

  (defun tab-by-id [tabs id]
    id)

  (defun selected-tab [tabs selected-tab]
    (tab-by-id tabs (selected-tab-id tabs selected-tab)))
                )");

  auto result = runtime.eval("(selected-tab [] {:id :settings})");

  EXPECT_EQ(result->to_string(), ":settings");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_same_named_function_in_value_position)
{
  runtime.eval(R"(
  (defun selected-tab [tabs selected-tab]
    selected-tab)
                )");

  auto result = runtime.eval("(selected-tab [] {:id :settings})");

  EXPECT_EQ(result->to_string(), "{:id :settings}");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_same_named_function_inside_let_body)
{
  runtime.eval(R"(
  (defun selected-tab [tabs selected-tab]
    (let [id (:id selected-tab)]
      selected-tab))
                )");

  auto result = runtime.eval("(selected-tab [] {:id :settings})");

  EXPECT_EQ(result->to_string(), "{:id :settings}");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_same_named_function_inside_nested_function)
{
  runtime.eval(R"(
  (defun selected-tab [tabs selected-tab]
    ((fn [] selected-tab)))
                )");

  auto result = runtime.eval("(selected-tab [] {:id :settings})");

  EXPECT_EQ(result->to_string(), "{:id :settings}");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_same_named_function_inside_nested_function_argument)
{
  runtime.eval(R"(
  (defun selected-tab-id [tabs selected-tab]
    (:id selected-tab))

  (defun selected-tab [tabs selected-tab]
    ((fn []
       (selected-tab-id tabs selected-tab))))
                )");

  auto result = runtime.eval("(selected-tab [] {:id :settings})");

  EXPECT_EQ(result->to_string(), ":settings");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_imported_function_in_value_position)
{
  runtime.eval(R"(
  (ns my-app.argument-shadowing-source)

  (defun selected-tab [tab]
    :imported-selected-tab)
                )");

  runtime.eval(R"(
  (ns my-app.argument-shadowing-consumer
    (:require my-app.argument-shadowing-source))

  (defun current-tab [selected-tab]
    selected-tab)
                )");

  auto result = runtime.eval("(current-tab {:id :settings})");

  EXPECT_EQ(result->to_string(), "{:id :settings}");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_imported_function_in_callee_position)
{
  runtime.eval(R"(
  (ns my-app.argument-callee-shadowing-source)

  (defun selected-tab [tab]
    :imported-selected-tab)
                )");

  runtime.eval(R"(
  (ns my-app.argument-callee-shadowing-consumer
    (:require my-app.argument-callee-shadowing-source))

  (defun current-tab [selected-tab]
    (selected-tab {:id :settings}))
                )");

  auto result = runtime.eval("(current-tab (fn [tab] (:id tab)))");

  EXPECT_EQ(result->to_string(), ":settings");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_imported_function_inside_nested_function_callee)
{
  runtime.eval(R"(
  (ns my-app.nested-argument-callee-shadowing-source)

  (defun selected-tab [tab]
    :imported-selected-tab)
                )");

  runtime.eval(R"(
  (ns my-app.nested-argument-callee-shadowing-consumer
    (:require my-app.nested-argument-callee-shadowing-source))

  (defun current-tab [selected-tab]
    ((fn []
       (selected-tab {:id :settings}))))
                )");

  auto result = runtime.eval("(current-tab (fn [tab] (:id tab)))");

  EXPECT_EQ(result->to_string(), ":settings");
}

TEST_F(Semantic_FunctionArgumentShadowing,
       argument_shadows_builtin_function_inside_nested_function_callee)
{
  runtime.eval(R"(
  (defun call-append [append]
    ((fn []
       (append [1] 2))))
                )");

  auto result = runtime.eval("(call-append (fn [items item] :parameter-append))");

  EXPECT_EQ(result->to_string(), ":parameter-append");
}


#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ResolveFunction = RooTest::RuntimeTestFixture;
TEST_F(ResolveFunction, resolve)
{
  // Given
  auto rt_lookup_result = runtime.lookup(*Roo::Value::symbol("concat"));

  // When
  auto result = runtime.eval("(resolve 'concat)");

  // Then
  ASSERT_EQ(result->type, Roo::Value::Type::FUNCTION);
  ASSERT_EQ(&result->exec(), &rt_lookup_result->exec());
}

TEST_F(ResolveFunction, runtime_lookup_falls_back_to_builtin_when_symbol_is_not_bound)
{
  runtime.eval("(ns my-app.clean-runtime-lookup)");

  auto append = runtime.lookup("append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), "[1 2]");
}

TEST_F(ResolveFunction, context_lookup_falls_back_to_builtin_when_symbol_is_not_bound)
{
  runtime.eval("(ns my-app.clean-context-lookup)");

  auto append = ctx.lookup("append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), "[1 2]");
}

TEST_F(ResolveFunction, bare_symbol_falls_back_to_builtin_when_symbol_is_not_bound)
{
  auto result = runtime.eval(R"(
  (ns my-app.clean-symbol-lookup)

  (def f append)
  (f [1] 2)
                )");

  EXPECT_EQ(result->to_string(), "[1 2]");
}

TEST_F(ResolveFunction, runtime_lookup_prefers_local_function_over_builtin)
{
  runtime.eval(R"(
  (ns my-app.runtime-lookup-shadow)

  (defun append [items item]
    :local-append)
                )");

  auto append = runtime.lookup("append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), ":local-append");
}

TEST_F(ResolveFunction, context_lookup_prefers_local_function_over_builtin)
{
  runtime.eval(R"(
  (ns my-app.context-lookup-shadow)

  (defun append [items item]
    :local-append)
                )");

  auto append = ctx.lookup("append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), ":local-append");
}

TEST_F(ResolveFunction, nil_resolves_to_nil)
{
  // Given
  auto result = runtime.eval("(resolve nil)");

  // Then
  ASSERT_EQ(*result, *Roo::Constant::NIL);
}

TEST_F(ResolveFunction, resolve_other_namespace)
{
  // Given
  runtime.ns("some.nested.space", true)->store("magic-number", Roo::Value::number(3));

  // When
  auto result = runtime.eval("(resolve 'some.nested.space/magic-number)");

  // Then
  EXPECT_EQ(*result, *Roo::Value::number(3));
  EXPECT_EQ(result->to_string(), "3");
}


#include <exception>
#include <memory>

#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/namespace.h>
#include <lisple/runtime.h>
#include <lisple/scope.h>
#include <lisple/type.h>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using Context = LispleTest::RuntimeTestFixture;
TEST_F(Context, lang_lookup)
{
  // When
  auto def = ctx.lookup(*Lisple::Value::symbol("def"));
  auto defun = ctx.lookup("defun");
  auto include = ctx.lookup(*Lisple::Value::symbol("include"));

  // Then
  EXPECT_EQ(def->type, Lisple::Value::Type::FUNCTION);
  EXPECT_EQ(defun->type, Lisple::Value::Type::FUNCTION);
  EXPECT_EQ(include->type, Lisple::Value::Type::FUNCTION);
}

TEST_F(Context, scope_lookup)
{
  // Given
  Lisple::sptr_val ns_string = Lisple::Value::string("ns genocide forget heaven");
  Lisple::sptr_val local_string =
    Lisple::Value::string("this is a local shop for local people");

  Lisple::Scope sub_scope;
  sub_scope.store("my-local-var", local_string);
  ctx.push_context(true, sub_scope);

  ctx.store_namespace("my-ns-var", ns_string);

  // When
  auto global_ctx_ref = ctx.lookup(*Lisple::Value::symbol("my-ns-var"));
  auto local_ctx_ref = ctx.lookup("my-local-var");
  auto non_global_ctx_ref = runtime.get_current_namespace().lookup("my-local-var");

  // Then
  EXPECT_EQ(*global_ctx_ref, *ns_string);
  EXPECT_EQ(*local_ctx_ref, *local_string);
  EXPECT_FALSE(non_global_ctx_ref.get());
}

TEST_F(Context, scoped_vars_go_away_when_context_is_popped)
{
  // Given
  Lisple::sptr_val local_string =
    Lisple::Value::string("this is a local shop for local people");

  Lisple::Scope local_scope;
  local_scope.store("my-local-var", local_string);

  ctx.push_context(true, local_scope);

  // When
  auto before_pop = ctx.lookup("my-local-var");
  Lisple::sptr_val after_pop;
  ;
  ctx.pop_context();
  try
  {
    after_pop = ctx.lookup("my-local-var");
  }
  catch (std::exception& e)
  {
    // This is the happy path!
  }

  // Then
  EXPECT_TRUE(before_pop.get());
  EXPECT_FALSE(after_pop.get());
}

TEST_F(Context, detached_context_preserves_scope)
{
  // Given
  Lisple::Scope scope1;
  Lisple::sptr_val string1 = Lisple::Value::string("string");
  scope1.store("thing", string1);
  ;
  ctx.push_context(true, scope1);

  ctx.push_context(true);

  Lisple::Scope scope2;
  Lisple::sptr_val string2 = Lisple::Value::string("another one");
  scope2.store("swamp-thing", string2);
  ctx.push_context(true, scope2);

  // When
  std::shared_ptr<Lisple::Context> detached = ctx.detach();

  // Then
  EXPECT_EQ(*detached->lookup(*Lisple::Value::symbol("thing")), *string1);
  EXPECT_EQ(*detached->lookup("swamp-thing"), *string2);

  EXPECT_EQ(*ctx.lookup("thing"), *string1);
  EXPECT_EQ(*ctx.lookup(*Lisple::Value::symbol("swamp-thing")), *string2);
}

TEST_F(Context, call_invokes_named_function_with_single_value_arg)
{
  // When
  auto result = ctx.call("inc", Lisple::Value::number(41));

  // Then
  EXPECT_EQ(result->i64(), 42);
}

TEST_F(Context, call_invokes_named_function_with_value_args)
{
  // Given
  Lisple::sptr_val_v args{Lisple::Value::number(10), Lisple::Value::number(32)};

  // When
  auto result = ctx.call("+", args);

  // Then
  EXPECT_EQ(result->i64(), 42);
}

TEST_F(Context, call_honors_current_namespace_aliases)
{
  // Given
  runtime.eval("(ns other)");
  runtime.eval("(defun add-one [x] (+ x 1))");
  runtime.eval("(ns main (:require [other :as o]))");

  // When
  auto result = ctx.call("o/add-one", Lisple::Value::number(41));

  // Then
  EXPECT_EQ(result->i64(), 42);
}

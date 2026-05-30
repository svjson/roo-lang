
#include <exception>
#include <memory>

#include <roo/context.h>
#include <roo/form.h>
#include <roo/namespace.h>
#include <roo/runtime.h>
#include <roo/scope.h>
#include <roo/type.h>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using Context = RooTest::RuntimeTestFixture;
TEST_F(Context, lang_lookup)
{
  // When
  auto def = ctx.lookup(*Roo::Value::symbol("def"));
  auto defun = ctx.lookup("defun");
  auto include = ctx.lookup(*Roo::Value::symbol("include"));

  // Then
  EXPECT_EQ(def->type, Roo::Value::Type::FUNCTION);
  EXPECT_EQ(defun->type, Roo::Value::Type::FUNCTION);
  EXPECT_EQ(include->type, Roo::Value::Type::FUNCTION);
}

TEST_F(Context, scope_lookup)
{
  // Given
  Roo::sptr_val ns_string = Roo::Value::string("ns genocide forget heaven");
  Roo::sptr_val local_string = Roo::Value::string("this is a local shop for local people");

  Roo::Scope sub_scope;
  sub_scope.store("my-local-var", local_string);
  ctx.push_context(true, sub_scope);

  ctx.store_namespace("my-ns-var", ns_string);

  // When
  auto global_ctx_ref = ctx.lookup(*Roo::Value::symbol("my-ns-var"));
  auto local_ctx_ref = ctx.lookup("my-local-var");
  auto non_global_ctx_ref = runtime.get_current_namespace().find("my-local-var");

  // Then
  EXPECT_EQ(*global_ctx_ref, *ns_string);
  EXPECT_EQ(*local_ctx_ref, *local_string);
  EXPECT_EQ(non_global_ctx_ref, nullptr);
}

TEST_F(Context, scoped_vars_go_away_when_context_is_popped)
{
  // Given
  Roo::sptr_val local_string = Roo::Value::string("this is a local shop for local people");

  Roo::Scope local_scope;
  local_scope.store("my-local-var", local_string);

  ctx.push_context(true, local_scope);

  // When
  auto before_pop = ctx.lookup("my-local-var");
  Roo::sptr_val after_pop;
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
  Roo::Scope scope1;
  Roo::sptr_val string1 = Roo::Value::string("string");
  scope1.store("thing", string1);
  ;
  ctx.push_context(true, scope1);

  ctx.push_context(true);

  Roo::Scope scope2;
  Roo::sptr_val string2 = Roo::Value::string("another one");
  scope2.store("swamp-thing", string2);
  ctx.push_context(true, scope2);

  // When
  std::shared_ptr<Roo::Context> detached = ctx.detach();

  // Then
  EXPECT_EQ(*detached->lookup(*Roo::Value::symbol("thing")), *string1);
  EXPECT_EQ(*detached->lookup("swamp-thing"), *string2);

  EXPECT_EQ(*ctx.lookup("thing"), *string1);
  EXPECT_EQ(*ctx.lookup(*Roo::Value::symbol("swamp-thing")), *string2);
}

TEST_F(Context, call_invokes_named_function_with_single_value_arg)
{
  // When
  auto result = ctx.call("inc", Roo::Value::number(41));

  // Then
  EXPECT_EQ(result->i64(), 42);
}

TEST_F(Context, call_invokes_named_function_with_value_args)
{
  // Given
  Roo::sptr_val_v args{Roo::Value::number(10), Roo::Value::number(32)};

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
  auto result = ctx.call("o/add-one", Roo::Value::number(41));

  // Then
  EXPECT_EQ(result->i64(), 42);
}

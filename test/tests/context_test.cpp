
#include "runtime_fixture.h"
#include <exception>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/namespace.h>
#include <lisple/runtime.h>
#include <lisple/scope.h>
#include <lisple/type.h>
#include <memory>


using Context = LispleTest::RuntimeTestFixture;
TEST_F(Context, lang_lookup)
{
  // Given
  Lisple::Word def_sym("def");
  Lisple::Word defun_sym("defun");
  Lisple::Word include_sym("include");

  // When
  auto def = ctx.lookup_value(def_sym);
  auto defun = ctx.lookup_value(defun_sym);
  auto include = ctx.lookup_value(include_sym);

  // Then
  EXPECT_EQ(def->type, Lisple::RTValue::Type::FUNCTION);
  EXPECT_EQ(defun->type, Lisple::RTValue::Type::FUNCTION);
  EXPECT_EQ(include->type, Lisple::RTValue::Type::FUNCTION);
}

TEST_F(Context, scope_lookup)
{
  // Given
  Lisple::Word ns_var_sym = Lisple::Word("my-ns-var");
  Lisple::Word local_var_sym = Lisple::Word("my-local-var");

  Lisple::sptr_rtval ns_string =
    Lisple::RTValue::string("ns genocide forget heaven");
  Lisple::sptr_rtval local_string =
    Lisple::RTValue::string("this is a local shop for local people");

  Lisple::Scope sub_scope;
  sub_scope.store(local_var_sym.value, local_string);
  ctx.push_context(true, sub_scope);

  ctx.store_namespace(ns_var_sym.value, ns_string);

  // When
  auto global_ctx_ref = ctx.lookup_value(ns_var_sym);
  auto local_ctx_ref = ctx.lookup_value(local_var_sym);
  auto non_global_ctx_ref = runtime.get_current_namespace().lookup_symbol(local_var_sym);

  // Then
  EXPECT_EQ(*global_ctx_ref, *ns_string);
  EXPECT_EQ(*local_ctx_ref, *local_string);
  EXPECT_FALSE(non_global_ctx_ref.get());
}

TEST_F(Context, scoped_vars_go_away_when_context_is_popped)
{
  // Given
  Lisple::Word local_var_sym = Lisple::Word("my-local-var");
  Lisple::sptr_rtval local_string =
    Lisple::RTValue::string("this is a local shop for local people");

  Lisple::Scope local_scope;
  local_scope.store(local_var_sym.value, local_string);

  ctx.push_context(true, local_scope);

  // When
  auto before_pop = ctx.lookup_value(local_var_sym);
  Lisple::sptr_rtval after_pop;
  ;
  ctx.pop_context();
  try
  {
    after_pop = ctx.lookup_value(local_var_sym);
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
  Lisple::Word identifier1("thing");
  Lisple::Word identifier2("swamp-thing");

  Lisple::Scope scope1;
  Lisple::sptr_rtval string1 = Lisple::RTValue::string("string");
  scope1.store(identifier1.value, string1);
  ;
  ctx.push_context(true, scope1);

  ctx.push_context(true);

  Lisple::Scope scope2;
  Lisple::sptr_rtval string2 = Lisple::RTValue::string("another one");
  scope2.store(identifier2.value, string2);
  ctx.push_context(true, scope2);

  // When
  std::shared_ptr<Lisple::Context> detached = ctx.detach();

  // Then
  EXPECT_EQ(*detached->lookup_value(identifier1), *string1);
  EXPECT_EQ(*detached->lookup_value(identifier2), *string2);

  EXPECT_EQ(*ctx.lookup_value(identifier1), *string1);
  EXPECT_EQ(*ctx.lookup_value(identifier2), *string2);
}

TEST_F(Context, call_invokes_named_function_with_single_rtvalue_arg)
{
  // When
  auto result = ctx.call("inc", Lisple::RTValue::number(41));

  // Then
  EXPECT_EQ(result->i64(), 42);
}

TEST_F(Context, call_invokes_named_function_with_rtvalue_args)
{
  // Given
  Lisple::sptr_rtval_v args{Lisple::RTValue::number(10), Lisple::RTValue::number(32)};

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
  auto result = ctx.call("o/add-one", Lisple::RTValue::number(41));

  // Then
  EXPECT_EQ(result->i64(), 42);
}

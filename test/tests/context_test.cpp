
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

TEST(Context, lang_lookup)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::Context ctx(fixture.runtime);
  Lisple::Word def_sym("def");
  Lisple::Word defun_sym("defun");
  Lisple::Word include_sym("include");

  // When
  auto def = ctx.lookup(def_sym);
  auto defun = ctx.lookup(defun_sym);
  auto include = ctx.lookup(include_sym);

  // Then
  EXPECT_EQ(def->get_type(), Lisple::Form::FUNCTION);
  EXPECT_EQ(defun->get_type(), Lisple::Form::FUNCTION);
  EXPECT_EQ(include->get_type(), Lisple::Form::FUNCTION);
}

TEST(Context, scope_lookup)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::Context ctx(fixture.runtime);

  Lisple::Word ns_var_sym = Lisple::Word("my-ns-var");
  Lisple::Word local_var_sym = Lisple::Word("my-local-var");

  Lisple::sptr_sobject ns_string =
    std::make_shared<Lisple::String>("ns genocide forget heaven");
  Lisple::sptr_sobject local_string =
    std::make_shared<Lisple::String>("this is a local shop for local people");

  Lisple::Scope sub_scope;
  sub_scope.store(local_var_sym, local_string);
  ctx.push_context(true, sub_scope);

  ctx.store_namespace(ns_var_sym, ns_string);

  // When
  auto global_ctx_ref = ctx.lookup(ns_var_sym);
  auto local_ctx_ref = ctx.lookup(local_var_sym);
  auto non_global_ctx_ref = fixture.runtime.get_current_namespace().lookup(local_var_sym);

  // Then
  EXPECT_EQ(*global_ctx_ref, *ns_string);
  EXPECT_EQ(*local_ctx_ref, *local_string);
  EXPECT_FALSE(non_global_ctx_ref.get());
}

TEST(Context, scoped_vars_go_away_when_context_is_popped)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::Context ctx(fixture.runtime);

  Lisple::Word local_var_sym = Lisple::Word("my-local-var");
  Lisple::sptr_sobject local_string =
    std::make_shared<Lisple::String>("this is a local shop for local people");

  Lisple::Scope local_scope;
  local_scope.store(local_var_sym, local_string);

  ctx.push_context(true, local_scope);

  // When
  auto before_pop = ctx.lookup(local_var_sym);
  Lisple::sptr_sobject after_pop;
  ;
  ctx.pop_context();
  try
  {
    after_pop = ctx.lookup(local_var_sym);
  }
  catch (std::exception& e)
  {
    // This is the happy path!
  }

  // Then
  EXPECT_TRUE(before_pop.get());
  EXPECT_FALSE(after_pop.get());
}

TEST(Context, detached_context_preserves_scope)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  Lisple::Word identifier1("thing");
  Lisple::Word identifier2("swamp-thing");

  Lisple::Scope scope1;
  Lisple::sptr_sobject string1 = std::make_shared<Lisple::String>("string");
  scope1.store(identifier1, string1);
  ;
  fixture.ctx.push_context(true, scope1);

  fixture.ctx.push_context(true);

  Lisple::Scope scope2;
  Lisple::sptr_sobject string2 = std::make_shared<Lisple::String>("another one");
  scope2.store(identifier2, string2);
  fixture.ctx.push_context(true, scope2);

  // When
  std::shared_ptr<Lisple::Context> detached = fixture.ctx.detach();

  // Then
  EXPECT_EQ(*detached->lookup(identifier1), *string1);
  EXPECT_EQ(*detached->lookup(identifier2), *string2);

  EXPECT_EQ(*fixture.ctx.lookup(identifier1), *string1);
  EXPECT_EQ(*fixture.ctx.lookup(identifier2), *string2);
}

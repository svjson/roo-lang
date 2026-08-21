#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ModuleFunction = RooTest::RuntimeTestFixture;

TEST_F(ModuleFunction, returns_live_view_with_symbol_keys)
{
  runtime.eval("(ns my-app.actions.move)");
  runtime.eval("(def speed 1)");
  runtime.eval("(ns caller)");
  runtime.eval(R"((def actions
                    (let [module-name 'my-app.actions.move
                          mode :live]
                      (module module-name mode))))");

  EXPECT_EQ(*runtime.eval("(get actions 'speed)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(head (keys actions))"), *Roo::Value::symbol("speed"));

  runtime.eval("(ns my-app.actions.move)");
  runtime.eval("(set! [speed] 2)");
  runtime.eval("(def execute! (fn [args] args))");
  runtime.eval("(ns caller)");

  EXPECT_EQ(*runtime.eval("(get actions 'speed)"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(contains? (keys actions) 'execute!)"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(apply (get actions 'execute!) [42])"), *Roo::Value::number(42));
}

TEST_F(ModuleFunction, live_view_rejects_mutation)
{
  runtime.eval("(ns my-app.actions.move)");
  runtime.eval("(def speed 1)");
  runtime.eval("(ns caller)");
  runtime.eval("(def actions (module 'my-app.actions.move))");

  EXPECT_THROW(runtime.eval("(assoc! actions 'speed 2)"), Roo::InvocationException);

  runtime.eval("(ns my-app.actions.move)");
  EXPECT_EQ(*runtime.eval("speed"), *Roo::Value::number(1));
}

TEST_F(ModuleFunction, snapshot_is_an_independent_mutable_copy)
{
  runtime.eval("(ns my-app.actions.move)");
  runtime.eval("(def speed 1)");
  runtime.eval("(ns caller)");
  runtime.eval(R"((def actions
                    (let [mode :snapshot]
                      (module 'my-app.actions.move mode))))");

  runtime.eval("(ns my-app.actions.move)");
  runtime.eval("(set! [speed] 2)");
  runtime.eval("(def execute! (fn [args] args))");
  runtime.eval("(ns caller)");

  EXPECT_EQ(*runtime.eval("(get actions 'speed)"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(get actions 'execute!)"), *Roo::Constant::NIL);
  runtime.eval("(assoc! actions 'local 3)");
  EXPECT_EQ(*runtime.eval("(get actions 'local)"), *Roo::Value::number(3));

  runtime.eval("(ns my-app.actions.move)");
  EXPECT_EQ(*runtime.eval("(get (module 'my-app.actions.move) 'local)"),
            *Roo::Constant::NIL);
}

TEST_F(ModuleFunction, rejects_unknown_mode)
{
  runtime.eval("(ns my-app.actions.move)");
  runtime.eval("(def speed 1)");

  EXPECT_THROW(runtime.eval("(module 'my-app.actions.move :current)"),
               Roo::InvocationException);
}

TEST_F(ModuleFunction, rejects_missing_namespace_without_creating_it)
{
  EXPECT_THROW(runtime.eval("(module 'missing.namespace)"), Roo::NamespaceException);
  EXPECT_EQ(runtime.ns("missing.namespace"), nullptr);
}

TEST_F(ModuleFunction, exposes_live_and_snapshot_signatures)
{
  const Roo::sptr_val module = runtime.eval("module");

  EXPECT_EQ(module->exec().get_signatures().size(), 2);
}

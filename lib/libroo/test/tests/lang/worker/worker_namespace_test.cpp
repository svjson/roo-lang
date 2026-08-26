#include <roo/exception.h>
#include <roo/runtime.h>
#include <roo/runtime/pretty_print.h>

#include <thread>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RooTest::RuntimeTestFixture;

TEST_F(RuntimeTestFixture, worker_create_starts_named_runtime_owned_worker)
{
  Roo::sptr_val result = runtime.eval("(roo.worker/create! :my-app/worker)");

  EXPECT_EQ(result, Roo::Constant::NIL);
}

TEST_F(RuntimeTestFixture, worker_identity_must_be_unique_within_parent_runtime)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");

  EXPECT_THROW(runtime.eval("(roo.worker/create! :my-app/worker)"), Roo::RooException);
}

TEST_F(RuntimeTestFixture, worker_identity_is_local_to_parent_runtime)
{
  Roo::Runtime another_runtime;

  EXPECT_NO_THROW(runtime.eval("(roo.worker/create! :my-app/worker)"));
  EXPECT_NO_THROW(another_runtime.eval("(roo.worker/create! :my-app/worker)"));
}

TEST_F(RuntimeTestFixture, worker_namespace_is_immutable)
{
  EXPECT_THROW(runtime.eval("(ns roo.worker) (def x 1)"), Roo::NamespaceException);
}

TEST_F(RuntimeTestFixture, worker_invoke_transfers_arguments_and_collects_the_result)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");
  runtime.eval(R"(
    (def shared {:answer [42]})
    (def execution
      (roo.worker/invoke! :my-app/worker identity [shared shared]))
  )");

  EXPECT_EQ(*runtime.eval("(:worker execution)"),
            *Roo::Value::keyword("my-app/worker"));
  EXPECT_EQ(*runtime.eval("(:id execution)"), *Roo::Value::number(1));
  Roo::Pretty::PrintOptions print_options;
  print_options.width = 80;
  EXPECT_EQ(Roo::Pretty::print(*runtime.eval("execution"), print_options),
            "{:worker :my-app/worker :id 1}");

  Roo::sptr_val status;
  do
  {
    status = runtime.eval("(roo.worker/poll! execution)");
    std::this_thread::yield();
  } while (status->str() == "queued" || status->str() == "running");

  ASSERT_EQ(status->str(), "succeeded");
  Roo::sptr_val result = runtime.eval("(roo.worker/collect! execution)");
  ASSERT_EQ(result->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(result->elements().size(), 2);
  EXPECT_EQ(result->elements()[0], result->elements()[1]);
  EXPECT_EQ(*result, *Roo::Value::vector({runtime.eval("shared"), runtime.eval("shared")}));
  EXPECT_THROW(runtime.eval("(roo.worker/poll! execution)"), Roo::RooException);
  EXPECT_THROW(runtime.eval("(roo.worker/collect! execution)"), Roo::RooException);
}

TEST_F(RuntimeTestFixture, worker_invoke_rejects_stateful_callable_before_enqueueing)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");

  EXPECT_THROW(
    runtime.eval("(roo.worker/invoke! :my-app/worker (fn [value] value) 42)"),
    Roo::RooException);
}

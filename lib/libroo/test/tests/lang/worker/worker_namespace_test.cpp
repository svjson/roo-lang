#include <thread>

#include <roo/exception.h>
#include <roo/runtime.h>
#include <roo/runtime/pretty_print.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RooTest::RuntimeTestFixture;

namespace
{
  Roo::sptr_val wait_for_worker_execution(Roo::Runtime& runtime,
                                          const std::string& execution)
  {
    Roo::sptr_val status;
    do
    {
      status = runtime.eval("(:status (roo.worker/poll! " + execution + "))");
      std::this_thread::yield();
    } while (status->str() == "queued" || status->str() == "running");
    return status;
  }
} // namespace

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

  EXPECT_EQ(*runtime.eval("(:worker execution)"), *Roo::Value::keyword("my-app/worker"));
  EXPECT_EQ(*runtime.eval("(:id execution)"), *Roo::Value::number(1));
  Roo::Pretty::PrintOptions print_options;
  print_options.width = 80;
  EXPECT_EQ(Roo::Pretty::print(*runtime.eval("execution"), print_options),
            "{:worker :my-app/worker :id 1}");

  EXPECT_EQ(runtime.eval("(roo.worker/poll! execution)")->type, Roo::Value::Type::MAP);
  Roo::sptr_val status = wait_for_worker_execution(runtime, "execution");

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

  EXPECT_THROW(runtime.eval("(roo.worker/invoke! :my-app/worker (fn [value] value) 42)"),
               Roo::RooException);
}

TEST_F(RuntimeTestFixture, worker_execute_let_evaluates_sequential_bindings_in_parent)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");
  runtime.eval(R"(
    (def parent-only 40)
    (def execution
      (roo.worker/execute-let! :my-app/worker
        [x parent-only
         y (+ x 2)]
        [x y (+ x y)]))
  )");

  ASSERT_EQ(wait_for_worker_execution(runtime, "execution")->str(), "succeeded");
  EXPECT_EQ(*runtime.eval("(roo.worker/collect! execution)"),
            *Roo::Value::vector(
              {Roo::Value::number(40), Roo::Value::number(42), Roo::Value::number(82)}));
}

TEST_F(RuntimeTestFixture, worker_execute_let_transfers_binding_roots_as_one_graph)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");
  runtime.eval(R"(
    (def shared {:answer [42]})
    (def execution
      (roo.worker/execute-let! :my-app/worker
        [x shared y shared]
        [x y]))
  )");

  ASSERT_EQ(wait_for_worker_execution(runtime, "execution")->str(), "succeeded");
  Roo::sptr_val result = runtime.eval("(roo.worker/collect! execution)");
  ASSERT_EQ(result->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(result->elements().size(), 2);
  EXPECT_EQ(result->elements()[0], result->elements()[1]);
}

TEST_F(RuntimeTestFixture, worker_execute_let_does_not_resolve_parent_names_in_body)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");
  runtime.eval(R"(
    (def parent-only 42)
    (def execution
      (roo.worker/execute-let! :my-app/worker [] parent-only))
  )");

  EXPECT_EQ(wait_for_worker_execution(runtime, "execution")->str(), "failed");
  EXPECT_THROW(runtime.eval("(roo.worker/collect! execution)"), Roo::RooException);
}

TEST_F(RuntimeTestFixture, worker_execute_let_rethrows_transferred_roo_error_map)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");
  runtime.eval(R"(
    (def execution
      (roo.worker/execute-let! :my-app/worker []
        (raise :my-app/worker-failure "Expected worker failure" {:job-id 42})))
  )");

  ASSERT_EQ(wait_for_worker_execution(runtime, "execution")->str(), "failed");
  EXPECT_EQ(*runtime.eval(R"(
      (guard
        ((:my-app/worker-failure [error] [(:type error) (:job-id error)]))
        (roo.worker/collect! execution))
    )"),
            *Roo::Value::vector(
              {Roo::Value::keyword("my-app/worker-failure"), Roo::Value::number(42)}));
}

TEST_F(RuntimeTestFixture, worker_failure_transfer_falls_back_without_stopping_worker)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");
  runtime.eval(R"(
    (def failed
      (roo.worker/execute-let! :my-app/worker []
        (raise "Original failure" {:callback (fn [] nil)})))
    (def later
      (roo.worker/execute-let! :my-app/worker [] 42))
  )");

  ASSERT_EQ(wait_for_worker_execution(runtime, "failed")->str(), "failed");
  ASSERT_EQ(wait_for_worker_execution(runtime, "later")->str(), "succeeded");
  Roo::sptr_val message = runtime.eval(R"(
    (guard
      ((:roo/error [error] (:message error)))
      (roo.worker/collect! failed))
  )");
  EXPECT_NE(message->str().find("Worker failure could not be transferred"),
            std::string::npos);
  EXPECT_EQ(*runtime.eval("(roo.worker/collect! later)"), *Roo::Value::number(42));
}

TEST_F(RuntimeTestFixture, worker_execute_let_rejects_unsafe_binding_before_enqueueing)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");

  EXPECT_THROW(runtime.eval(R"(
      (roo.worker/execute-let! :my-app/worker
        [callable (fn [] nil)]
        (callable))
    )"),
               Roo::RooException);
}

TEST_F(RuntimeTestFixture, worker_execute_let_preserves_source_for_persisted_worker_code)
{
  runtime.eval("(roo.worker/create! :my-app/worker)");
  runtime.eval(R"(
    (def definition
      (roo.worker/execute-let! :my-app/worker []
        (def fail-in-worker (fn [] (+ :not-a-number 1)))
        nil))
  )",
               "worker-definition.roo");

  ASSERT_EQ(wait_for_worker_execution(runtime, "definition")->str(), "succeeded");
  EXPECT_EQ(runtime.eval("(roo.worker/collect! definition)"), Roo::Constant::NIL);

  runtime.eval(R"(
    (def invocation
      (roo.worker/execute-let! :my-app/worker []
        (fail-in-worker)))
  )",
               "worker-invocation.roo");

  ASSERT_EQ(wait_for_worker_execution(runtime, "invocation")->str(), "failed");
  try
  {
    runtime.eval("(roo.worker/collect! invocation)");
    FAIL() << "Expected the persisted worker function to fail.";
  }
  catch (const Roo::RaisedError& error)
  {
    EXPECT_NE(std::string(error.what()).find("worker-definition.roo:"), std::string::npos)
      << error.what();
  }
}

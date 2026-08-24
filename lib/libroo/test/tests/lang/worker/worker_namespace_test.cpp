#include <roo/exception.h>
#include <roo/runtime.h>

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

#include "runtime_fixture.h"

#include <lisple/exception.h>

#include <gtest/gtest.h>

using IoNamespace = LispleTest::RuntimeTestFixture;

TEST_F(IoNamespace, can_be_required_with_alias)
{
  fs.add_file("data.edn", "[:ok 1]");

  EXPECT_EQ(runtime
              .eval(R"(
    (ns my-app.core (:require [lisple.io :as io]))
    (io/slurp-edn! "data.edn")
  )")
              ->to_string(),
            "[:ok 1]");
}

TEST_F(IoNamespace, cannot_be_modified_from_lisple)
{
  EXPECT_THROW(runtime.eval("(ns lisple.io) (def x 1)"), Lisple::NamespaceException);
}

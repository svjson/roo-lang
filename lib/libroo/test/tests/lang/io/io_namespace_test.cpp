#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IoNamespace = RooTest::RuntimeTestFixture;

TEST_F(IoNamespace, can_be_required_with_alias)
{
  fs.add_file("data.edn", "[:ok 1]");

  EXPECT_EQ(runtime
              .eval(R"(
    (ns my-app.core (:require [roo.io :as io]))
    (io/slurp-edn! "data.edn")
  )")
              ->to_string(),
            "[:ok 1]");
}

TEST_F(IoNamespace, lisple_io_can_be_required_as_legacy_alias)
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

TEST_F(IoNamespace, cannot_be_modified_from_roo)
{
  EXPECT_THROW(runtime.eval("(ns roo.io) (def x 1)"), Roo::NamespaceException);
}

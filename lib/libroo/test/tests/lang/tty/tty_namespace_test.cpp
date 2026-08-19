#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using TtyNamespace = RooTest::RuntimeTestFixture;

TEST_F(TtyNamespace, can_be_required_with_alias)
{
  // Given/When
  const std::string result = runtime
                                .eval(R"(
    (ns my-app.core (:require [roo.tty :as tty]))
    (tty/tty?)
  )")
                                ->to_string();

  // Then
  EXPECT_TRUE(result == "true" || result == "false");
}

TEST_F(TtyNamespace, cannot_be_modified_from_roo)
{
  // Then
  EXPECT_THROW(runtime.eval("(ns roo.tty) (def x 1)"), Roo::NamespaceException);
}

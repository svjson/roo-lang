#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using StringNamespace = RooTest::RuntimeTestFixture;

TEST_F(StringNamespace, can_be_required_with_alias)
{
  EXPECT_EQ(*runtime.eval(R"(
    (ns my-app.core (:require [roo.string :as string]))
    (string/starts-with? "Normal Club" "Normal")
  )"),
            *Roo::Constant::BOOL_TRUE);
}

TEST_F(StringNamespace, cannot_be_modified_from_roo)
{
  EXPECT_THROW(runtime.eval("(ns roo.string) (def x 1)"), Roo::NamespaceException);
}

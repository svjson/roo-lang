#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AstNamespace = RooTest::RuntimeTestFixture;

TEST_F(AstNamespace, can_be_required_with_alias)
{
  fs.add_file("sample.roo", "(def x 1)");

  EXPECT_EQ(runtime
              .eval(R"(
    (ns my-app.core (:require [roo.ast :as ast]))
    (count (ast/slurp! "sample.roo"))
  )")
              ->to_string(),
            "1");
}

TEST_F(AstNamespace, cannot_be_modified_from_roo)
{
  EXPECT_THROW(runtime.eval("(ns roo.ast) (def x 1)"), Roo::NamespaceException);
}

TEST_F(AstNamespace, read_string_reports_each_keyword_occurrence_source)
{
  auto result = runtime.eval(R"(
    (let [nodes (:children (head (roo.ast/read-string "[:same :same]")))]
      [(get-in nodes [0 :source :column])
       (get-in nodes [1 :source :column])])
  )");

  ASSERT_EQ(result->to_string(), "[2 8]");
}

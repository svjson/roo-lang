#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AstSlurpBang = RooTest::RuntimeTestFixture;

TEST_F(AstSlurpBang, slurps_source_file_as_ast_nodes)
{
  fs.add_file("sample.roo", "(ns sample.core)\n(def answer 42)\n");

  EXPECT_EQ(runtime.eval(R"((count (roo.ast/slurp! "sample.roo")))")->to_string(), "2");
  EXPECT_EQ(runtime.eval(R"((:type (nth (roo.ast/slurp! "sample.roo") 0)))")->to_string(),
            ":list");
  EXPECT_EQ(
    runtime.eval(R"((str (:form (nth (roo.ast/slurp! "sample.roo") 1))))")->to_string(),
    "\"(def answer 42)\"");
  EXPECT_EQ(
    runtime.eval(R"((:path (:source (nth (roo.ast/slurp! "sample.roo") 0))))")->to_string(),
    R"("sample.roo")");
}

TEST_F(AstSlurpBang, includes_nested_ast_source_locations)
{
  fs.add_file("sample.roo", "(ns sample.core)\n(defun f []\n  (do 1))\n");

  EXPECT_EQ(runtime
              .eval(R"(
    (let [forms (roo.ast/slurp! "sample.roo")
          defun-node (nth forms 1)
          body-node (nth (:children defun-node) 3)]
      (:line (:source body-node)))
  )")
              ->to_string(),
            "3");
  EXPECT_EQ(runtime
              .eval(R"(
    (let [forms (roo.ast/slurp! "sample.roo")
          defun-node (nth forms 1)
          body-node (nth (:children defun-node) 3)]
      (:column (:source body-node)))
  )")
              ->to_string(),
            "3");
}

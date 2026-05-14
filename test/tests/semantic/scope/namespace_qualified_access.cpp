#include <gtest/gtest.h>
#include "runtime_fixture.h"


using Semantic_NamespaceQualifiedAccess = LispleTest::RuntimeTestFixture;
TEST_F(Semantic_NamespaceQualifiedAccess,
     symbols_from_foreign_namespace_are_available_when_fully_qualified)
{
  // Given
  runtime.eval(R"(

  (ns my-app.util)

  (defun reverse-string [input]
    (reduce input "" (fn [r c] (str c r))))
                )");

  runtime.eval(R"(
  (ns my-app.domain)

  (def MAGIC-NUMBER 52)
                )");

  runtime.eval(R"(
  (ns my-app.core
    (:require my-app.util))
                )");

  // When
  runtime.switch_namespace("my-app.domain");

  // Then
  auto reversed = runtime.eval(R"((my-app.util/reverse-string "Caramba!"))");
  EXPECT_EQ(reversed->to_string(), R"("!abmaraC")");
}

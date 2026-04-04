

#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(Semantic_NamespaceInclude, symbols_from_included_unaliased_namespace_are_available)
{
  // Given
  Lisple::Runtime runtime;
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
  runtime.switch_namespace("my-app.core");

  // Then
  auto reversed = runtime.eval(R"((reverse-string "Caramba!"))");

  EXPECT_EQ(reversed->to_string(), R"("!abmaraC")");
}

TEST(Semantic_NamespaceInclude,
     function_from_aliased_namespace_are_available_via_alias_qualifier)
{
  // Given
  Lisple::Runtime runtime;
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
    (:require [my-app.util :as util]))
                )");

  // When
  runtime.switch_namespace("my-app.core");

  // Then
  auto reversed = runtime.eval(R"((util/reverse-string "Caramba!"))");
  EXPECT_EQ(reversed->to_string(), R"("!abmaraC")");
}

TEST(Semantic_NamespaceInclude,
     symbol_from_aliased_namespace_are_available_via_alias_qualifier)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"(
  (ns my-app.domain)

  (def head-honcho {:name "Bubby" :role "Bosmang"})
                )");

  runtime.eval(R"(
  (ns my-app.core
    (:require [my-app.domain :as domain]))
                )");

  // When
  runtime.switch_namespace("my-app.core");

  // Then
  auto name_prop = runtime.eval(R"((let [nm (:name domain/head-honcho)] nm))");
  EXPECT_EQ(name_prop->to_string(), R"("Bubby")");
}

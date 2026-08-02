

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_NamespaceInclude = RooTest::RuntimeTestFixture;
TEST_F(Semantic_NamespaceInclude, symbols_from_included_unaliased_namespace_are_available)
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
  runtime.switch_namespace("my-app.core");

  // Then
  auto reversed = runtime.eval(R"((reverse-string "Caramba!"))");

  EXPECT_EQ(reversed->to_string(), R"("!abmaraC")");
}

TEST_F(Semantic_NamespaceInclude,
       function_from_aliased_namespace_are_available_via_alias_qualifier)
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
    (:require [my-app.util :as util]))
                )");

  // When
  runtime.switch_namespace("my-app.core");

  // Then
  auto reversed = runtime.eval(R"((util/reverse-string "Caramba!"))");
  EXPECT_EQ(reversed->to_string(), R"("!abmaraC")");
}

TEST_F(Semantic_NamespaceInclude,
       symbol_from_aliased_namespace_are_available_via_alias_qualifier)
{
  // Given
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

TEST_F(Semantic_NamespaceInclude, local_function_shadows_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.local-shadow)

  (defun append [items item]
    :local-append)

  (defun call-append []
    (append [1] 2))
                )");

  auto result = runtime.eval("(call-append)");

  EXPECT_EQ(result->to_string(), ":local-append");
}

TEST_F(Semantic_NamespaceInclude, aliased_userspace_function_shadows_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.shadow-source)

  (defun append [items item]
    :aliased-append)
                )");

  runtime.eval(R"(
  (ns my-app.shadow-consumer
    (:require [my-app.shadow-source :as source]))

  (defun call-append []
    (source/append [1] 2))
                )");

  auto result = runtime.eval("(call-append)");

  EXPECT_EQ(result->to_string(), ":aliased-append");
}

TEST_F(Semantic_NamespaceInclude, local_function_value_shadows_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.local-value-shadow)

  (defun append [items item]
    :local-value-append)

  (def f append)

  (defun call-f []
    (f [1] 2))
                )");

  auto result = runtime.eval("(call-f)");

  EXPECT_EQ(result->to_string(), ":local-value-append");
}

TEST_F(Semantic_NamespaceInclude,
       resolve_prefers_local_function_over_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.resolve-local-shadow)

  (defun append [items item]
    :resolved-local-append)

  (defun call-resolved []
    ((resolve 'append) [1] 2))
                )");

  auto result = runtime.eval("(call-resolved)");

  EXPECT_EQ(result->to_string(), ":resolved-local-append");
}

TEST_F(Semantic_NamespaceInclude,
       higher_order_function_uses_local_function_over_global_builtin)
{
  runtime.eval(R"(
  (ns my-app.higher-order-local-shadow)

  (defun append [item]
    :higher-order-local-append)

  (defun call-map []
    (map [1] append))
                )");

  auto result = runtime.eval("(call-map)");

  EXPECT_EQ(result->to_string(), "[:higher-order-local-append]");
}

TEST_F(Semantic_NamespaceInclude, lexical_binding_shadows_global_builtin_function)
{
  auto result = runtime.eval(R"(
  (let [append (fn [items item] :lexical-append)]
    (append [1] 2))
                )");

  EXPECT_EQ(result->to_string(), ":lexical-append");
}

TEST_F(Semantic_NamespaceInclude, function_parameter_shadows_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.parameter-shadow)

  (defun call-with [append]
    (append [1] 2))
                )");

  auto result = runtime.eval(R"((call-with (fn [items item] :parameter-append)))");

  EXPECT_EQ(result->to_string(), ":parameter-append");
}

TEST_F(Semantic_NamespaceInclude,
       anonymous_function_parameter_shadows_global_builtin_function)
{
  auto result = runtime.eval(R"(
  ((fn [append]
     (append [1] 2))
   (fn [items item] :anonymous-parameter-append))
                )");

  EXPECT_EQ(result->to_string(), ":anonymous-parameter-append");
}

TEST_F(Semantic_NamespaceInclude,
       later_let_binding_sees_previous_function_named_like_global_builtin)
{
  auto result = runtime.eval(R"(
  (let [append (fn [items item] :previous-let-append)
        result (append [1] 2)]
    result)
                )");

  EXPECT_EQ(result->to_string(), ":previous-let-append");
}

TEST_F(Semantic_NamespaceInclude, map_destructured_binding_shadows_global_builtin_function)
{
  auto result = runtime.eval(R"(
  (let [{:keys [append]} {:append (fn [items item] :map-destructured-append)}]
    (append [1] 2))
                )");

  EXPECT_EQ(result->to_string(), ":map-destructured-append");
}

TEST_F(Semantic_NamespaceInclude,
       vector_destructured_binding_shadows_global_builtin_function)
{
  auto result = runtime.eval(R"(
  (let [[append] [(fn [items item] :vector-destructured-append)]]
    (append [1] 2))
                )");

  EXPECT_EQ(result->to_string(), ":vector-destructured-append");
}

TEST_F(Semantic_NamespaceInclude, loop_binding_shadows_global_builtin_function)
{
  auto result = runtime.eval(R"(
  (for [append [(fn [items item] :loop-append)]]
    (append [1] 2))
                )");

  EXPECT_EQ(result->to_string(), "[:loop-append]");
}

TEST_F(Semantic_NamespaceInclude,
       unaliased_imported_function_shadows_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.import-shadow-source)

  (defun append [items item]
    :imported-append)
                )");

  runtime.eval(R"(
  (ns my-app.import-shadow-consumer
    (:require my-app.import-shadow-source))

  (defun call-append []
    (append [1] 2))
                )");

  auto result = runtime.eval("(call-append)");

  EXPECT_EQ(result->to_string(), ":imported-append");
}

TEST_F(Semantic_NamespaceInclude,
       runtime_lookup_prefers_unaliased_import_over_global_builtin)
{
  runtime.eval(R"(
  (ns my-app.runtime-lookup-import-source)

  (defun append [items item]
    :imported-append)
                )");

  runtime.eval(R"(
  (ns my-app.runtime-lookup-import-consumer
    (:require my-app.runtime-lookup-import-source))
                )");

  auto append = runtime.lookup("append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), ":imported-append");
}

TEST_F(Semantic_NamespaceInclude,
       resolve_prefers_unaliased_imported_function_over_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.resolve-import-shadow-source)

  (defun append [items item]
    :resolved-imported-append)
                )");

  runtime.eval(R"(
  (ns my-app.resolve-import-shadow-consumer
    (:require my-app.resolve-import-shadow-source))

  (defun call-resolved []
    ((resolve 'append) [1] 2))
                )");

  auto result = runtime.eval("(call-resolved)");

  EXPECT_EQ(result->to_string(), ":resolved-imported-append");
}

TEST_F(Semantic_NamespaceInclude,
       context_lookup_prefers_unaliased_import_over_global_builtin)
{
  runtime.eval(R"(
  (ns my-app.context-lookup-import-source)

  (defun append [items item]
    :imported-append)
                )");

  runtime.eval(R"(
  (ns my-app.context-lookup-import-consumer
    (:require my-app.context-lookup-import-source))
                )");

  auto append = ctx.lookup("append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), ":imported-append");
}

TEST_F(Semantic_NamespaceInclude, aliased_import_does_not_shadow_unqualified_global_builtin)
{
  runtime.eval(R"(
  (ns my-app.no-alias-leak-source)

  (defun append [items item]
    :aliased-append)
                )");

  runtime.eval(R"(
  (ns my-app.no-alias-leak-consumer
    (:require [my-app.no-alias-leak-source :as source]))

  (defun call-append []
    (append [1] 2))
                )");

  auto result = runtime.eval("(call-append)");

  EXPECT_EQ(result->to_string(), "[1 2]");
}

TEST_F(Semantic_NamespaceInclude,
       alias_qualifier_takes_precedence_over_global_namespace_name)
{
  runtime.eval(R"(
  (ns util)

  (defun append [items item]
    :global-util-append)
                )");

  runtime.eval(R"(
  (ns my-app.alias-collision-source)

  (defun append [items item]
    :aliased-collision-append)
                )");

  runtime.eval(R"(
  (ns my-app.alias-collision-consumer
    (:require [my-app.alias-collision-source :as util]))

  (defun call-append []
    (util/append [1] 2))
                )");

  auto result = runtime.eval("(call-append)");

  EXPECT_EQ(result->to_string(), ":aliased-collision-append");
}

TEST_F(Semantic_NamespaceInclude, global_namespace_qualifier_resolves_when_no_alias_exists)
{
  runtime.eval(R"(
  (ns util.no-alias)

  (defun append [items item]
    :global-util-append)
                )");

  runtime.eval(R"(
  (ns my-app.global-namespace-consumer)

  (defun call-append []
    (util.no-alias/append [1] 2))
                )");

  auto result = runtime.eval("(call-append)");

  EXPECT_EQ(result->to_string(), ":global-util-append");
}

TEST_F(Semantic_NamespaceInclude,
       runtime_lookup_prefers_alias_qualifier_over_global_namespace_name)
{
  runtime.eval(R"(
  (ns runtime-util)

  (defun append [items item]
    :global-util-append)
                )");

  runtime.eval(R"(
  (ns my-app.runtime-alias-collision-source)

  (defun append [items item]
    :aliased-collision-append)
                )");

  runtime.eval(R"(
  (ns my-app.runtime-alias-collision-consumer
    (:require [my-app.runtime-alias-collision-source :as runtime-util]))
                )");

  auto append = runtime.lookup("runtime-util/append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), ":aliased-collision-append");
}

TEST_F(Semantic_NamespaceInclude,
       context_lookup_prefers_alias_qualifier_over_global_namespace_name)
{
  runtime.eval(R"(
  (ns context-util)

  (defun append [items item]
    :global-util-append)
                )");

  runtime.eval(R"(
  (ns my-app.context-alias-collision-source)

  (defun append [items item]
    :aliased-collision-append)
                )");

  runtime.eval(R"(
  (ns my-app.context-alias-collision-consumer
    (:require [my-app.context-alias-collision-source :as context-util]))
                )");

  auto append = ctx.lookup("context-util/append");
  Roo::sptr_val_v args = {runtime.eval("[1]"), Roo::Value::number(2)};
  auto result = append->exec().execute(ctx, args);

  EXPECT_EQ(result->to_string(), ":aliased-collision-append");
}

TEST_F(Semantic_NamespaceInclude,
       resolve_prefers_alias_qualifier_over_global_namespace_name)
{
  runtime.eval(R"(
  (ns resolve-util)

  (defun append [items item]
    :global-util-append)
                )");

  runtime.eval(R"(
  (ns my-app.resolve-alias-collision-source)

  (defun append [items item]
    :aliased-collision-append)
                )");

  runtime.eval(R"(
  (ns my-app.resolve-alias-collision-consumer
    (:require [my-app.resolve-alias-collision-source :as resolve-util]))

  (defun call-resolved []
    ((resolve 'resolve-util/append) [1] 2))
                )");

  auto result = runtime.eval("(call-resolved)");

  EXPECT_EQ(result->to_string(), ":aliased-collision-append");
}

TEST_F(Semantic_NamespaceInclude,
       qualified_function_value_prefers_alias_qualifier_over_global_namespace_name)
{
  runtime.eval(R"(
  (ns value-util)

  (defun append [items item]
    :global-util-append)
                )");

  runtime.eval(R"(
  (ns my-app.value-alias-collision-source)

  (defun append [items item]
    :aliased-collision-append)
                )");

  runtime.eval(R"(
  (ns my-app.value-alias-collision-consumer
    (:require [my-app.value-alias-collision-source :as value-util]))

  (def f value-util/append)

  (defun call-f []
    (f [1] 2))
                )");

  auto result = runtime.eval("(call-f)");

  EXPECT_EQ(result->to_string(), ":aliased-collision-append");
}

TEST_F(Semantic_NamespaceInclude, explicit_roo_qualifier_resolves_global_builtin_function)
{
  runtime.eval(R"(
  (ns my-app.explicit-roo-shadow)

  (defun append [items item]
    :local-append)

  (defun call-roo-append []
    (roo/append [1] 2))
                )");

  auto result = runtime.eval("(call-roo-append)");

  EXPECT_EQ(result->to_string(), "[1 2]");
}

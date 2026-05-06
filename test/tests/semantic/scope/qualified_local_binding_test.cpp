#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(QualifiedSymbol, locally_bound_qualified_symbol_resolves_from_scope)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(let [player/bullets 42] player/bullets)");

  // Then
  ASSERT_EQ(result->i64(), 42);
}

TEST(QualifiedSymbol, map_destructuring_into_qualified_local_names)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval(
    "(let [{:keys [player/bullets]} {:player/bullets 7}] player/bullets)");

  // Then
  ASSERT_EQ(result->i64(), 7);
}

TEST(QualifiedSymbol, qualified_local_shadows_namespace_binding)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(ns player) (def bullets 99) (ns user)");

  // When
  auto result = runtime.eval("(let [player/bullets 42] player/bullets)");

  // Then
  ASSERT_EQ(result->i64(), 42);
}

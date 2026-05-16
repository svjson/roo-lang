
#include <memory>

#include <lisple/form.h>
#include <lisple/scope.h>

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace Lisple;
using namespace Lisple::AST;

TEST(Scope, has__no_value)
{
  // Given
  Scope scope;

  // When
  bool result = scope.has("ident");

  // Then
  ASSERT_FALSE(result);
}

TEST(Scope, has__after_store)
{
  // Given
  Scope scope;

  // When
  scope.store("ident", Lisple::Value::number(10));
  bool result = scope.has("ident");

  // Then
  ASSERT_TRUE(result);
}

TEST(Scope, find__missing_returns_nullptr)
{
  // Given
  Scope scope;

  // Then
  ASSERT_EQ(scope.find("ident"), nullptr);
}

TEST(Scope, lookup__missing_throws)
{
  // Given
  Scope scope;

  // Then
  ASSERT_THROW(scope.lookup("ident"), Lisple::IdentifierException);
}

TEST(Scope, lookup__existing_returns_stored_value_reference)
{
  // Given
  Scope scope;
  Lisple::sptr_val value = Lisple::Value::number(10);
  scope.store("ident", value);

  // When
  const Lisple::sptr_val& result = scope.lookup("ident");

  // Then
  ASSERT_EQ(&result, scope.find("ident"));
}

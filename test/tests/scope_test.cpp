
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
  scope.store("ident", Lisple::RTValue::number(10));
  bool result = scope.has("ident");

  // Then
  ASSERT_TRUE(result);
}

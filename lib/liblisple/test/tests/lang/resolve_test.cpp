
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ResolveFunction = LispleTest::RuntimeTestFixture;
TEST_F(ResolveFunction, resolve)
{
  // Given
  auto rt_lookup_result = runtime.lookup(*Lisple::Value::symbol("concat"));

  // When
  auto result = runtime.eval("(resolve 'concat)");

  // Then
  ASSERT_EQ(result->type, Lisple::Value::Type::FUNCTION);
  ASSERT_EQ(&result->exec(), &rt_lookup_result->exec());
}

TEST_F(ResolveFunction, nil_resolves_to_nil)
{
  // Given
  auto result = runtime.eval("(resolve nil)");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST_F(ResolveFunction, resolve_other_namespace)
{
  // Given
  runtime.ns("some.nested.space", true)->store("magic-number", Lisple::Value::number(3));

  // When
  auto result = runtime.eval("(resolve 'some.nested.space/magic-number)");

  // Then
  EXPECT_EQ(*result, *Lisple::Value::number(3));
  EXPECT_EQ(result->to_string(), "3");
}

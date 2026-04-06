
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(ResolveFunction, resolve)
{
  // Given
  Lisple::Runtime runtime;
  auto rt_lookup_result = runtime.lookup_value(Lisple::Word("concat"));

  // When
  auto result = runtime.eval("(resolve 'concat)");

  // Then
  ASSERT_EQ(result->type, Lisple::RTValue::Type::FUNCTION);
  ASSERT_EQ(&result->exec(), &rt_lookup_result->exec());
}

TEST(ResolveFunction, nil_resolves_to_nil)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(resolve nil)");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST(ResolveFunction, resolve_other_namespace)
{
  // Given
  Lisple::Runtime runtime;
  runtime.ns("some.nested.space", true)->store("magic-number", Lisple::RTValue::number(3));

  // When
  auto result = runtime.eval("(resolve 'some.nested.space/magic-number)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(3));
  EXPECT_EQ(result->to_string(), "3");
}

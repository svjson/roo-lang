
#include <lisple/lang/base.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(DefForm, exec_def__string)
{
  // Given
  Lisple::Runtime runtime;

  // When
  runtime.eval("(def var-name \"Var Value\")");

  // Then
  auto obj = runtime.get_current_namespace().lookup_value("var-name");
  ASSERT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefForm, define_var)
{
  // Given
  Lisple::Runtime runtime;

  // When
  runtime.eval("(def var-name \"Var Value\")");

  // Then
  auto obj = runtime.get_current_namespace().lookup_value("var-name");
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefForm, define_var_with_gt_and_lt)
{
  // Given
  Lisple::Runtime runtime;

  // When
  runtime.eval("(def <var-name> \"Value\")");

  // Then
  auto obj = runtime.get_current_namespace().lookup_value("<var-name>");
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefForm, def_returns_defined_value)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto defined = runtime.eval("(def my-var {:a 1000 :b 2000})");

  // Then
  ASSERT_EQ(defined->to_string(), "{:a 1000 :b 2000}");
}


#include <roo/lang/base.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DefForm = RooTest::RuntimeTestFixture;
TEST_F(DefForm, exec_def__string)
{
  // Given
  runtime.eval("(def var-name \"Var Value\")");

  // Then
  auto obj = runtime.get_current_namespace().lookup("var-name");
  ASSERT_TRUE(obj.get());
  EXPECT_TRUE(Roo::Type::STRING.is_type_of(*obj));
}

TEST_F(DefForm, define_var)
{
  // Given
  runtime.eval("(def var-name \"Var Value\")");

  // Then
  auto obj = runtime.get_current_namespace().lookup("var-name");
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Roo::Type::STRING.is_type_of(*obj));
}

TEST_F(DefForm, define_var_with_gt_and_lt)
{
  // Given
  runtime.eval("(def <var-name> \"Value\")");

  // Then
  auto obj = runtime.get_current_namespace().lookup("<var-name>");
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Roo::Type::STRING.is_type_of(*obj));
}

TEST_F(DefForm, def_returns_defined_value)
{
  // Given
  auto defined = runtime.eval("(def my-var {:a 1000 :b 2000})");

  // Then
  ASSERT_EQ(defined->to_string(), "{:a 1000 :b 2000}");
}

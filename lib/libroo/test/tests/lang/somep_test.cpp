
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SomePFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * SomePFunction - (some? value)
 * Returns true if value is not nil, false if nil.
 * ======================================================================
 */

TEST_F(SomePFunction, returns_false_for_nil)
{
  // Given
  auto result = runtime.eval("(some? nil)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_FALSE);
}

TEST_F(SomePFunction, returns_true_for_string)
{
  // Given
  auto result = runtime.eval(R"((some? "hello"))");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(SomePFunction, returns_true_for_number)
{
  // Given
  auto result = runtime.eval("(some? 0)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(SomePFunction, returns_true_for_false)
{
  // Given
  auto result = runtime.eval("(some? false)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(SomePFunction, returns_true_for_empty_string)
{
  // Given
  auto result = runtime.eval(R"((some? ""))");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(SomePFunction, returns_true_for_empty_vector)
{
  // Given
  auto result = runtime.eval("(some? [])");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(SomePFunction, returns_true_for_keyword)
{
  // Given
  auto result = runtime.eval("(some? :nil)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

TEST_F(SomePFunction, returns_true_for_map)
{
  // Given
  auto result = runtime.eval("(some? {:key \"value\"})");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::BOOL_TRUE);
}

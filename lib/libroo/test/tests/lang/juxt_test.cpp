
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using JuxtFunction = RooTest::RuntimeTestFixture;

TEST_F(JuxtFunction, returns_a_callable_value)
{
  // When
  auto retval = runtime.eval("(juxt inc dec)");

  // Then
  ASSERT_EQ(retval->type, Roo::Value::Type::FUNCTION);
}

TEST_F(JuxtFunction, applies_each_function_and_returns_vector)
{
  // When
  auto retval = runtime.eval("((juxt inc dec) 5)");

  // Then
  ASSERT_TRUE(Roo::Type::VECTOR.is_type_of(*retval));
  ASSERT_EQ(Roo::count(*retval), 2);
  EXPECT_EQ(retval->elements().at(0)->i64(), 6);
  EXPECT_EQ(retval->elements().at(1)->i64(), 4);
}

TEST_F(JuxtFunction, preserves_function_order_in_result)
{
  // When
  auto retval = runtime.eval("((juxt dec identity inc) 10)");

  // Then
  ASSERT_EQ(Roo::count(*retval), 3);
  EXPECT_EQ(retval->elements().at(0)->i64(), 9);
  EXPECT_EQ(retval->elements().at(1)->i64(), 10);
  EXPECT_EQ(retval->elements().at(2)->i64(), 11);
}

TEST_F(JuxtFunction, works_with_keyword_accessors)
{
  // When
  auto retval = runtime.eval("((juxt :a :b) {:a 1 :b 2})");

  // Then
  ASSERT_EQ(Roo::count(*retval), 2);
  EXPECT_EQ(retval->elements().at(0)->i64(), 1);
  EXPECT_EQ(retval->elements().at(1)->i64(), 2);
}

TEST_F(JuxtFunction, single_function)
{
  // When
  auto retval = runtime.eval("((juxt inc) 7)");

  // Then
  ASSERT_EQ(Roo::count(*retval), 1);
  EXPECT_EQ(retval->elements().at(0)->i64(), 8);
}

TEST_F(JuxtFunction, can_be_stored_and_reused)
{
  // Given
  runtime.eval("(def f (juxt inc dec))");

  // When
  auto r1 = runtime.eval("(f 5)");
  auto r2 = runtime.eval("(f 10)");

  // Then
  EXPECT_EQ(r1->elements().at(0)->i64(), 6);
  EXPECT_EQ(r1->elements().at(1)->i64(), 4);
  EXPECT_EQ(r2->elements().at(0)->i64(), 11);
  EXPECT_EQ(r2->elements().at(1)->i64(), 9);
}

TEST_F(JuxtFunction, works_with_user_defined_functions)
{
  // Given
  runtime.eval("(defun double [n] (* n 2))");
  runtime.eval("(defun triple [n] (* n 3))");

  // When
  auto retval = runtime.eval("((juxt double triple) 4)");

  // Then
  EXPECT_EQ(retval->elements().at(0)->i64(), 8);
  EXPECT_EQ(retval->elements().at(1)->i64(), 12);
}

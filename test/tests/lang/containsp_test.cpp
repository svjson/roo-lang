#include "runtime_fixture.h"

#include <gtest/gtest.h>


using ContainsPredicateFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * ContainsPredicateFunction - (contains? [...] predicate)
 * ======================================================================
 */

TEST_F(ContainsPredicateFunction, contains_array)
{
  // Given
  runtime.eval("(def my-vec [1 3 5 6 7 8])");

  // Then
  EXPECT_EQ(*runtime.eval("(contains? my-vec 1)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 2)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 3)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 4)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 5)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 6)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 7)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 8)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 9)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 10)"), *Lisple::Constant::BOOL_FALSE);
}

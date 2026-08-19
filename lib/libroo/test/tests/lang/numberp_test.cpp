#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NumberPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(NumberPredicateFunction, accepts_one_or_more_numbers)
{
  EXPECT_EQ(*runtime.eval("(number? 0)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(number? 0 -4 3.5)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(NumberPredicateFunction, returns_false_when_any_value_is_not_a_number)
{
  EXPECT_EQ(*runtime.eval(R"((number? 0 "1" 2.0))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(number? nil)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(NumberPredicateFunction, requires_at_least_one_value)
{
  EXPECT_THROW(runtime.eval("(number?)"), Roo::NoMatchingSignatureException);
}

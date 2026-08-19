#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DigitPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(DigitPredicateFunction, accepts_digits_in_supported_forms)
{
  EXPECT_EQ(*runtime.eval(R"((digit? '8' "4" :1 '7 3))"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(DigitPredicateFunction, accepts_only_single_digit_text)
{
  EXPECT_EQ(*runtime.eval(R"((digit? "12" :12 '12))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval(R"((digit? "a" :a 'a))"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(DigitPredicateFunction, accepts_only_integer_numbers_from_zero_to_nine)
{
  EXPECT_EQ(*runtime.eval("(digit? 0 9)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(digit? -1)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(digit? 9.0)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(digit? 10)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(DigitPredicateFunction, rejects_nil)
{
  EXPECT_EQ(*runtime.eval("(digit? nil)"), *Roo::Constant::BOOL_FALSE);
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IntPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(IntPredicateFunction, accepts_integer_numbers)
{
  EXPECT_EQ(*runtime.eval("(int? 0 1 -2 2147483648)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(IntPredicateFunction, rejects_other_representations)
{
  EXPECT_EQ(*runtime.eval("(int? 1 2.0)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval(R"((int? 1 "2"))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(int? nil)"), *Roo::Constant::BOOL_FALSE);
}

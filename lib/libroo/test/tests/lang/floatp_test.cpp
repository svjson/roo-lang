#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FloatPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(FloatPredicateFunction, accepts_floating_point_numbers)
{
  EXPECT_EQ(*runtime.eval("(float? 0.0 1.5 -2.0)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(FloatPredicateFunction, rejects_other_representations)
{
  EXPECT_EQ(*runtime.eval("(float? 1.0 2)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval(R"((float? 1.0 "2.0"))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(float? nil)"), *Roo::Constant::BOOL_FALSE);
}


#include "runtime_fixture.h"

#include <gtest/gtest.h>


using OrForm = LispleTest::RuntimeTestFixture;
TEST_F(OrForm, logical_or)
{
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 3))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 3))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 1) (odd? 2))"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(or (odd? 2) (odd? 4))"), *Lisple::Constant::BOOL_FALSE);
}

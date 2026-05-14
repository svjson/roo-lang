
#include "runtime_fixture.h"

#include <gtest/gtest.h>

using AndForm = LispleTest::RuntimeTestFixture;

TEST_F(AndForm, logical_and)
{
  EXPECT_EQ(*runtime.eval("(and (odd? 1) (odd? 3))"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(and (odd? 2) (odd? 3))"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(and (odd? 1) (odd? 2))"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(and (odd? 2) (odd? 4))"), *Lisple::Constant::BOOL_FALSE);
}

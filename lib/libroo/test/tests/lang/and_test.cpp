
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AndForm = RooTest::RuntimeTestFixture;

TEST_F(AndForm, logical_and)
{
  EXPECT_EQ(*runtime.eval("(and (odd? 1) (odd? 3))"), *Roo::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(and (odd? 2) (odd? 3))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(and (odd? 1) (odd? 2))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(and (odd? 2) (odd? 4))"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(AndForm, no_args_returns_nil)
{
  auto result = runtime.eval("(and)");

  ASSERT_TRUE(result.get());
  ASSERT_EQ(result, Roo::Constant::NIL);
}

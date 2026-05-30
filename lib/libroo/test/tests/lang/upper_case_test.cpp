
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using UpperCaseFunction = RooTest::RuntimeTestFixture;
TEST_F(UpperCaseFunction, uppercase)
{
  // Given
  EXPECT_EQ(*runtime.eval("(upper-case \"mIxEd-CaSe!\")"),
            *Roo::Value::string("MIXED-CASE!"));
  EXPECT_EQ(*runtime.eval("(upper-case \"The King is dead.\")"),
            *Roo::Value::string("THE KING IS DEAD."));
  EXPECT_EQ(*runtime.eval("(upper-case :regular-key)"), *Roo::Value::string(":REGULAR-KEY"));
  EXPECT_EQ(*runtime.eval("(upper-case {:x 10 :y 8})"), *Roo::Value::string("{:X 10 :Y 8}"));
  EXPECT_EQ(*runtime.eval("(upper-case nil)"), *Roo::Value::string("NIL"));
}

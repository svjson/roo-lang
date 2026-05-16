
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using UpperCaseFunction = LispleTest::RuntimeTestFixture;
TEST_F(UpperCaseFunction, uppercase)
{
  // Given
  EXPECT_EQ(*runtime.eval("(upper-case \"mIxEd-CaSe!\")"),
            *Lisple::Value::string("MIXED-CASE!"));
  EXPECT_EQ(*runtime.eval("(upper-case \"The King is dead.\")"),
            *Lisple::Value::string("THE KING IS DEAD."));
  EXPECT_EQ(*runtime.eval("(upper-case :regular-key)"),
            *Lisple::Value::string(":REGULAR-KEY"));
  EXPECT_EQ(*runtime.eval("(upper-case {:x 10 :y 8})"),
            *Lisple::Value::string("{:X 10 :Y 8}"));
  EXPECT_EQ(*runtime.eval("(upper-case nil)"), *Lisple::Value::string("NIL"));
}


#include "runtime_fixture.h"

#include <gtest/gtest.h>


using UpperCaseFunction = LispleTest::RuntimeTestFixture;
TEST_F(UpperCaseFunction, uppercase)
{
  // Given
  EXPECT_EQ(*runtime.eval("(upper-case \"mIxEd-CaSe!\")"),
            *Lisple::RTValue::string("MIXED-CASE!"));
  EXPECT_EQ(*runtime.eval("(upper-case \"The King is dead.\")"),
            *Lisple::RTValue::string("THE KING IS DEAD."));
  EXPECT_EQ(*runtime.eval("(upper-case :regular-key)"),
            *Lisple::RTValue::string(":REGULAR-KEY"));
  EXPECT_EQ(*runtime.eval("(upper-case {:x 10 :y 8})"),
            *Lisple::RTValue::string("{:X 10 :Y 8}"));
  EXPECT_EQ(*runtime.eval("(upper-case nil)"), *Lisple::RTValue::string("NIL"));
}

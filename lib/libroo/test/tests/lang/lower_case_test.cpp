
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using LowerCaseFunction = RooTest::RuntimeTestFixture;
TEST_F(LowerCaseFunction, lowercase)
{
  // Given
  EXPECT_EQ(*runtime.eval("(lower-case \"mIxEd-CaSe!\")"),
            *Roo::Value::string("mixed-case!"));
  EXPECT_EQ(*runtime.eval("(lower-case \"The King is dead.\")"),
            *Roo::Value::string("the king is dead."));
  EXPECT_EQ(*runtime.eval("(lower-case :UPCASE-KEY)"), *Roo::Value::string(":upcase-key"));
  EXPECT_EQ(*runtime.eval("(lower-case {:X 10 :Y 8})"), *Roo::Value::string("{:x 10 :y 8}"));
  EXPECT_EQ(*runtime.eval("(lower-case nil)"), *Roo::Value::string("nil"));
}

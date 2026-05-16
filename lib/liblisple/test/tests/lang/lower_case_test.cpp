
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using LowerCaseFunction = LispleTest::RuntimeTestFixture;
TEST_F(LowerCaseFunction, lowercase)
{
  // Given
  EXPECT_EQ(*runtime.eval("(lower-case \"mIxEd-CaSe!\")"),
            *Lisple::Value::string("mixed-case!"));
  EXPECT_EQ(*runtime.eval("(lower-case \"The King is dead.\")"),
            *Lisple::Value::string("the king is dead."));
  EXPECT_EQ(*runtime.eval("(lower-case :UPCASE-KEY)"),
            *Lisple::Value::string(":upcase-key"));
  EXPECT_EQ(*runtime.eval("(lower-case {:X 10 :Y 8})"),
            *Lisple::Value::string("{:x 10 :y 8}"));
  EXPECT_EQ(*runtime.eval("(lower-case nil)"), *Lisple::Value::string("nil"));
}

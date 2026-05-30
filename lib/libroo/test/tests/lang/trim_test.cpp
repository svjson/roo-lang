#include "runtime_fixture.h"
#include <gtest/gtest.h>

using TrimFunction = RooTest::RuntimeTestFixture;
TEST_F(TrimFunction, trims_leading_and_trailing_whitespace)
{
  // Given
  EXPECT_EQ(*runtime.eval("(trim \"  hello  \")"), *Roo::Value::string("hello"));
  EXPECT_EQ(*runtime.eval(R"((trim "
	 hello there 
"))"),
            *Roo::Value::string("hello there"));
  EXPECT_EQ(*runtime.eval("(trim \"\")"), *Roo::Value::string(""));
}

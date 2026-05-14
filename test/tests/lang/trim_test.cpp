#include "runtime_fixture.h"
#include <gtest/gtest.h>

using TrimFunction = LispleTest::RuntimeTestFixture;
TEST_F(TrimFunction, trims_leading_and_trailing_whitespace)
{
  // Given
  EXPECT_EQ(*runtime.eval("(trim \"  hello  \")"), *Lisple::Value::string("hello"));
  EXPECT_EQ(*runtime.eval(R"((trim "
	 hello there 
"))"),
            *Lisple::Value::string("hello there"));
  EXPECT_EQ(*runtime.eval("(trim \"\")"), *Lisple::Value::string(""));
}

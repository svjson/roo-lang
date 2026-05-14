#include "runtime_fixture.h"

#include <gtest/gtest.h>


using TrimFunction = LispleTest::RuntimeTestFixture;
TEST_F(TrimFunction, trims_leading_and_trailing_whitespace)
{
  // Given
  EXPECT_EQ(*runtime.eval("(trim \"  hello  \")"), *Lisple::RTValue::string("hello"));
  EXPECT_EQ(*runtime.eval(R"((trim "
	 hello there 
"))"),
            *Lisple::RTValue::string("hello there"));
  EXPECT_EQ(*runtime.eval("(trim \"\")"), *Lisple::RTValue::string(""));
}

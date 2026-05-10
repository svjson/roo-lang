#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(TrimFunction, trims_leading_and_trailing_whitespace)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(trim \"  hello  \")"), *Lisple::RTValue::string("hello"));
  EXPECT_EQ(*runtime.eval(R"((trim "
	 hello there 
"))"),
            *Lisple::RTValue::string("hello there"));
  EXPECT_EQ(*runtime.eval("(trim \"\")"), *Lisple::RTValue::string(""));
}

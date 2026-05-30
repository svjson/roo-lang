#include "runtime_fixture.h"
#include <gtest/gtest.h>

using BlankPFunction = RooTest::RuntimeTestFixture;
TEST_F(BlankPFunction, recognizes_nil_empty_and_whitespace_only_strings)
{
  // Given
  EXPECT_EQ(*runtime.eval("(blank? nil)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(blank? \"\")"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((blank? "
	  "))"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(blank? \"hello\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(blank? \" hello \")"), *Roo::Constant::BOOL_FALSE);
}

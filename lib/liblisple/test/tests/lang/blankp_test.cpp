#include "runtime_fixture.h"

#include <gtest/gtest.h>


using BlankPFunction = LispleTest::RuntimeTestFixture;
TEST_F(BlankPFunction, recognizes_nil_empty_and_whitespace_only_strings)
{
  // Given
  EXPECT_EQ(*runtime.eval("(blank? nil)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(blank? \"\")"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((blank? "
	  "))"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(blank? \"hello\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(blank? \" hello \")"), *Lisple::Constant::BOOL_FALSE);
}

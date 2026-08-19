#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CodepointFunction = RooTest::RuntimeTestFixture;

TEST_F(CodepointFunction, returns_character_codepoint)
{
  EXPECT_EQ(*runtime.eval("(codepoint 'g')"), *Roo::Value::number(103));
  EXPECT_EQ(*runtime.eval("(codepoint '8')"), *Roo::Value::number(56));
}

TEST_F(CodepointFunction, returns_escaped_character_codepoint)
{
  EXPECT_EQ(*runtime.eval(R"((codepoint '\n'))"), *Roo::Value::number(10));
}

TEST_F(CodepointFunction, returns_nil_for_nil_input)
{
  EXPECT_EQ(*runtime.eval("(codepoint nil)"), *Roo::Constant::NIL);
}

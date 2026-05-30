
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IntFunction = RooTest::RuntimeTestFixture;
TEST_F(IntFunction, char_to_int)
{
  // Given
  EXPECT_EQ(*runtime.eval("(int 'c')"), *Roo::Value::number(99));
  EXPECT_EQ(*runtime.eval("(int ' ')"), *Roo::Value::number(32));
}

TEST_F(IntFunction, float_to_int)
{
  // Given
  EXPECT_EQ(*runtime.eval("(int 12.0)"), *Roo::Value::number(12));
  EXPECT_EQ(*runtime.eval("(int 15.4)"), *Roo::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.5)"), *Roo::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.9)"), *Roo::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 16)"), *Roo::Value::number(16));
}

TEST_F(IntFunction, string_to_int)
{
  EXPECT_EQ(*runtime.eval(R"((int "16"))"), *Roo::Value::number(16));
  EXPECT_EQ(*runtime.eval(R"((int "-16"))"), *Roo::Value::number(-16));
  EXPECT_EQ(*runtime.eval(R"((int " 16 "))"), *Roo::Value::number(16));
}

TEST_F(IntFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(int nil)"), *Roo::Constant::NIL);
}

TEST_F(IntFunction, malformed_strings_return_nil)
{
  EXPECT_EQ(*runtime.eval(R"((int ""))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "abc"))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "12abc"))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "999999999999999999999999999999"))"), *Roo::Constant::NIL);
}


#include "runtime_fixture.h"
#include <lisple/exception.h>

#include <gtest/gtest.h>

using IntFunction = LispleTest::RuntimeTestFixture;
TEST_F(IntFunction, char_to_int)
{
  // Given
  EXPECT_EQ(*runtime.eval("(int 'c')"), *Lisple::Value::number(99));
  EXPECT_EQ(*runtime.eval("(int ' ')"), *Lisple::Value::number(32));
}

TEST_F(IntFunction, float_to_int)
{
  // Given
  EXPECT_EQ(*runtime.eval("(int 12.0)"), *Lisple::Value::number(12));
  EXPECT_EQ(*runtime.eval("(int 15.4)"), *Lisple::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.5)"), *Lisple::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 15.9)"), *Lisple::Value::number(15));
  EXPECT_EQ(*runtime.eval("(int 16)"), *Lisple::Value::number(16));
}

TEST_F(IntFunction, string_to_int)
{
  EXPECT_EQ(*runtime.eval(R"((int "16"))"), *Lisple::Value::number(16));
  EXPECT_EQ(*runtime.eval(R"((int "-16"))"), *Lisple::Value::number(-16));
  EXPECT_EQ(*runtime.eval(R"((int " 16 "))"), *Lisple::Value::number(16));
}

TEST_F(IntFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(int nil)"), *Lisple::Constant::NIL);
}

TEST_F(IntFunction, malformed_strings_return_nil)
{
  EXPECT_EQ(*runtime.eval(R"((int ""))"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "abc"))"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "12abc"))"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "999999999999999999999999999999"))"),
            *Lisple::Constant::NIL);
}

#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FloatFunction = RooTest::RuntimeTestFixture;

namespace
{
  void expect_float_value(const Roo::sptr_val& value, float expected)
  {
    ASSERT_NE(value, nullptr);
    ASSERT_EQ(value->type, Roo::Value::Type::NUMBER);
    EXPECT_EQ(value->num().num_type, Roo::Value::NumberType::FLOAT);
    EXPECT_FLOAT_EQ(value->num().get_float(), expected);
  }
} // namespace

TEST_F(FloatFunction, char_to_float)
{
  expect_float_value(runtime.eval("(float 'c')"), 99.0f);
  expect_float_value(runtime.eval("(float ' ')"), 32.0f);
}

TEST_F(FloatFunction, number_to_float)
{
  expect_float_value(runtime.eval("(float 12)"), 12.0f);
  expect_float_value(runtime.eval("(float 12.5)"), 12.5f);
}

TEST_F(FloatFunction, string_to_float)
{
  expect_float_value(runtime.eval(R"((float "16"))"), 16.0f);
  expect_float_value(runtime.eval(R"((float "-16.5"))"), -16.5f);
  expect_float_value(runtime.eval(R"((float " 16.25 "))"), 16.25f);
}

TEST_F(FloatFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(float nil)"), *Roo::Constant::NIL);
}

TEST_F(FloatFunction, malformed_strings_return_nil)
{
  EXPECT_EQ(*runtime.eval(R"((float ""))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((float "abc"))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((float "12abc"))"), *Roo::Constant::NIL);
}

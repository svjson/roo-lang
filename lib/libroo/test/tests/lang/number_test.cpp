#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NumberFunction = RooTest::RuntimeTestFixture;

namespace
{
  void expect_int(const Roo::sptr_val& value, int expected)
  {
    ASSERT_EQ(value->type, Roo::Value::Type::NUMBER);
    ASSERT_EQ(value->num().num_type, Roo::Value::NumberType::INT);
    EXPECT_EQ(value->num().int_value, expected);
  }

  void expect_float(const Roo::sptr_val& value, float expected)
  {
    ASSERT_EQ(value->type, Roo::Value::Type::NUMBER);
    ASSERT_EQ(value->num().num_type, Roo::Value::NumberType::FLOAT);
    EXPECT_FLOAT_EQ(value->num().float_value, expected);
  }
}

TEST_F(NumberFunction, preserves_existing_numbers)
{
  auto integer = runtime.eval("(number 4)");
  auto floating = runtime.eval("(number 4.5)");
  expect_int(integer, 4);
  expect_float(floating, 4.5f);
}

TEST_F(NumberFunction, parses_integer_and_float_text)
{
  expect_int(runtime.eval(R"((number "489"))"), 489);
  expect_float(runtime.eval(R"((number "48.12"))"), 48.12f);
  expect_int(runtime.eval("(number '8')"), 8);
  expect_int(runtime.eval(R"((number :1))"), 1);
  expect_int(runtime.eval(R"((number :-1))"), -1);
}

TEST_F(NumberFunction, invalid_values_return_nil)
{
  EXPECT_EQ(*runtime.eval(R"((number "not-a-number"))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(number nil)"), *Roo::Constant::NIL);
}

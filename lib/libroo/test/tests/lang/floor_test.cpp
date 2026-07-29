#include <cstdint>
#include <limits>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FloorFunction = RooTest::RuntimeTestFixture;

namespace
{
  Roo::sptr_val make_float(double value)
  {
    return Roo::Value::number(
      Roo::Value::Number{.num_type = Roo::Value::NumberType::FLOAT, .float_value = value});
  }

  void expect_long_value(const Roo::sptr_val& value, std::int64_t expected)
  {
    ASSERT_NE(value, nullptr);
    ASSERT_EQ(value->type, Roo::Value::Type::NUMBER);
    ASSERT_EQ(value->num().num_type, Roo::Value::NumberType::LONG);
    EXPECT_EQ(value->num().long_value, expected);
  }
} // namespace

TEST_F(FloorFunction, floor)
{
  EXPECT_EQ(*runtime.eval("(floor 10.8)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(floor 10.0)"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(floor 5)"), *Roo::Value::number(5));
  EXPECT_EQ(*runtime.eval("(floor 19.9)"), *Roo::Value::number(19));
  EXPECT_EQ(*runtime.eval("(floor -10.2)"), *Roo::Value::number(-11));
}

TEST_F(FloorFunction, returns_long_when_result_exceeds_int_range)
{
  runtime.get_current_namespace().store("large", make_float(2147483648.75));

  expect_long_value(runtime.eval("(floor large)"), 2147483648LL);
}

TEST_F(FloorFunction, rejects_non_finite_and_out_of_long_range_values)
{
  runtime.get_current_namespace().store("too-large", make_float(1.0e20));
  runtime.get_current_namespace().store("too-small", make_float(-1.0e20));
  runtime.get_current_namespace().store("infinite",
                                        make_float(std::numeric_limits<double>::infinity()));
  runtime.get_current_namespace().store(
    "not-a-number",
    make_float(std::numeric_limits<double>::quiet_NaN()));

  EXPECT_EQ(*runtime.eval("(floor too-large)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(floor too-small)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(floor infinite)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(floor not-a-number)"), *Roo::Constant::NIL);
}

TEST_F(FloorFunction, nil_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(floor nil)"), *Roo::Constant::NIL);
}

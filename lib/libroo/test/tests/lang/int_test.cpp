
#include <cstdint>
#include <limits>

#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IntFunction = RooTest::RuntimeTestFixture;

namespace
{
  Roo::sptr_val make_long(std::int64_t value)
  {
    return Roo::Value::number(
      Roo::Value::Number{.num_type = Roo::Value::NumberType::LONG, .long_value = value});
  }

  Roo::sptr_val make_float(double value)
  {
    return Roo::Value::number(
      Roo::Value::Number{.num_type = Roo::Value::NumberType::FLOAT, .float_value = value});
  }

  void expect_int_value(const Roo::sptr_val& value, std::int32_t expected)
  {
    ASSERT_NE(value, nullptr);
    ASSERT_EQ(value->type, Roo::Value::Type::NUMBER);
    ASSERT_EQ(value->num().num_type, Roo::Value::NumberType::INT);
    EXPECT_EQ(value->num().int_value, expected);
  }

  void expect_long_value(const Roo::sptr_val& value, std::int64_t expected)
  {
    ASSERT_NE(value, nullptr);
    ASSERT_EQ(value->type, Roo::Value::Type::NUMBER);
    ASSERT_EQ(value->num().num_type, Roo::Value::NumberType::LONG);
    EXPECT_EQ(value->num().long_value, expected);
  }
} // namespace

TEST_F(IntFunction, char_to_int)
{
  expect_int_value(runtime.eval("(int 'c')"), 99);
  expect_int_value(runtime.eval("(int ' ')"), 32);
}

TEST_F(IntFunction, number_to_int)
{
  expect_int_value(runtime.eval("(int 16)"), 16);
  expect_int_value(runtime.eval("(int -16)"), -16);
  expect_long_value(runtime.eval("(int 2147483648)"), 2147483648LL);
  expect_long_value(runtime.eval("(int -2147483649)"), -2147483649LL);
}

TEST_F(IntFunction, long_numbers_preserve_long_values)
{
  const std::int64_t above_int_max =
    static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max()) + 1;
  const std::int64_t below_int_min =
    static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::min()) - 1;
  const std::int64_t long_max = std::numeric_limits<std::int64_t>::max();
  const std::int64_t long_min = std::numeric_limits<std::int64_t>::min();

  runtime.get_current_namespace().store("above-int-max", make_long(above_int_max));
  runtime.get_current_namespace().store("below-int-min", make_long(below_int_min));
  runtime.get_current_namespace().store("long-max", make_long(long_max));
  runtime.get_current_namespace().store("long-min", make_long(long_min));

  expect_long_value(runtime.eval("(int above-int-max)"), above_int_max);
  expect_long_value(runtime.eval("(int below-int-min)"), below_int_min);
  expect_long_value(runtime.eval("(int long-max)"), long_max);
  expect_long_value(runtime.eval("(int long-min)"), long_min);
}

TEST_F(IntFunction, float_to_int)
{
  expect_int_value(runtime.eval("(int 12.0)"), 12);
  expect_int_value(runtime.eval("(int 15.4)"), 15);
  expect_int_value(runtime.eval("(int 15.5)"), 15);
  expect_int_value(runtime.eval("(int 15.9)"), 15);
  expect_int_value(runtime.eval("(int -15.9)"), -15);
}

TEST_F(IntFunction, float_to_int_uses_long_when_truncated_value_exceeds_int_range)
{
  const std::int64_t above_int_max =
    static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max()) + 1;
  const std::int64_t below_int_min =
    static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::min()) - 1;

  runtime.get_current_namespace().store("above-int-max", make_float(2147483648.75));
  runtime.get_current_namespace().store("below-int-min", make_float(-2147483649.75));

  expect_long_value(runtime.eval("(int above-int-max)"), above_int_max);
  expect_long_value(runtime.eval("(int below-int-min)"), below_int_min);
}

TEST_F(IntFunction, float_to_int_rejects_non_finite_and_out_of_long_range_values)
{
  runtime.get_current_namespace().store("too-large", make_float(1.0e20));
  runtime.get_current_namespace().store("too-small", make_float(-1.0e20));
  runtime.get_current_namespace().store("infinite",
                                        make_float(std::numeric_limits<double>::infinity()));
  runtime.get_current_namespace().store(
    "not-a-number",
    make_float(std::numeric_limits<double>::quiet_NaN()));

  EXPECT_EQ(*runtime.eval("(int too-large)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(int too-small)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(int infinite)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(int not-a-number)"), *Roo::Constant::NIL);
}

TEST_F(IntFunction, string_to_int)
{
  expect_int_value(runtime.eval(R"((int "16"))"), 16);
  expect_int_value(runtime.eval(R"((int "-16"))"), -16);
  expect_int_value(runtime.eval(R"((int " 16 "))"), 16);
}

TEST_F(IntFunction, string_to_int_preserves_long_values)
{
  expect_long_value(runtime.eval(R"((int "2147483648"))"), 2147483648LL);
  expect_long_value(runtime.eval(R"((int "-2147483649"))"), -2147483649LL);
  expect_long_value(runtime.eval(R"((int "9223372036854775807"))"),
                    std::numeric_limits<std::int64_t>::max());
  expect_long_value(runtime.eval(R"((int "-9223372036854775808"))"),
                    std::numeric_limits<std::int64_t>::min());
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
  EXPECT_EQ(*runtime.eval(R"((int "9223372036854775808"))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "-9223372036854775809"))"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval(R"((int "999999999999999999999999999999"))"), *Roo::Constant::NIL);
}

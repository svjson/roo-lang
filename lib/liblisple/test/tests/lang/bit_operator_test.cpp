#include <lisple/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using BitOperatorFunction = LispleTest::RuntimeTestFixture;

TEST_F(BitOperatorFunction, shift_left_defaults_to_one_bit)
{
  EXPECT_EQ(runtime.eval("(<< 8)")->to_string(), "16");
  EXPECT_EQ(runtime.eval("(<<< 8)")->to_string(), "16");
}

TEST_F(BitOperatorFunction, shift_left_accepts_explicit_bit_count)
{
  EXPECT_EQ(runtime.eval("(<< 8 2)")->to_string(), "32");
  EXPECT_EQ(runtime.eval("(<<< 8 2)")->to_string(), "32");
}

TEST_F(BitOperatorFunction, shift_right_defaults_to_one_bit)
{
  EXPECT_EQ(runtime.eval("(>> 8)")->to_string(), "4");
  EXPECT_EQ(runtime.eval("(>>> 8)")->to_string(), "4");
}

TEST_F(BitOperatorFunction, shift_right_accepts_explicit_bit_count)
{
  EXPECT_EQ(runtime.eval("(>> 8 3)")->to_string(), "1");
  EXPECT_EQ(runtime.eval("(>>> 8 3)")->to_string(), "1");
}

TEST_F(BitOperatorFunction, arithmetic_right_shift_preserves_sign)
{
  EXPECT_EQ(runtime.eval("(>> -8 1)")->to_string(), "-4");
  EXPECT_EQ(runtime.eval("(>> -8 2)")->to_string(), "-2");
}

TEST_F(BitOperatorFunction, logical_right_shift_zero_fills)
{
  EXPECT_EQ(runtime.eval("(>>> -1 1)")->to_string(), "9223372036854775807");
  EXPECT_EQ(runtime.eval("(>>> -8 1)")->to_string(), "9223372036854775804");
}

TEST_F(BitOperatorFunction, bit_and_accepts_one_or_more_integer_arguments)
{
  EXPECT_EQ(runtime.eval("(bit-and 6)")->to_string(), "6");
  EXPECT_EQ(runtime.eval("(bit-and 6 3)")->to_string(), "2");
  EXPECT_EQ(runtime.eval("(bit-and 15 7 3)")->to_string(), "3");
}

TEST_F(BitOperatorFunction, bit_or_accepts_one_or_more_integer_arguments)
{
  EXPECT_EQ(runtime.eval("(bit-or 4)")->to_string(), "4");
  EXPECT_EQ(runtime.eval("(bit-or 4 2)")->to_string(), "6");
  EXPECT_EQ(runtime.eval("(bit-or 8 4 1)")->to_string(), "13");
}

TEST_F(BitOperatorFunction, bit_xor_accepts_one_or_more_integer_arguments)
{
  EXPECT_EQ(runtime.eval("(bit-xor 6)")->to_string(), "6");
  EXPECT_EQ(runtime.eval("(bit-xor 6 3)")->to_string(), "5");
  EXPECT_EQ(runtime.eval("(bit-xor 7 3 1)")->to_string(), "5");
}

TEST_F(BitOperatorFunction, bit_not_complements_integer_bits)
{
  EXPECT_EQ(runtime.eval("(bit-not 0)")->to_string(), "-1");
  EXPECT_EQ(runtime.eval("(bit-not 10)")->to_string(), "-11");
}

TEST_F(BitOperatorFunction, bit_operators_reject_float_arguments)
{
  EXPECT_THROW(runtime.eval("(bit-and 1.5 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(bit-or 1.5 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(bit-xor 1.5 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(bit-not 1.5)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(<< 1.5 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(>> 1.5 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(<<< 1.5 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(>>> 1.5 1)"), Lisple::TypeError);
}

TEST_F(BitOperatorFunction, shift_operators_reject_invalid_shift_counts)
{
  EXPECT_THROW(runtime.eval("(<< 1 -1)"), Lisple::LispleException);
  EXPECT_THROW(runtime.eval("(>> 1 64)"), Lisple::LispleException);
  EXPECT_THROW(runtime.eval("(<<< 1 -1)"), Lisple::LispleException);
  EXPECT_THROW(runtime.eval("(>>> 1 64)"), Lisple::LispleException);
}

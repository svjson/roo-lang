
#include <roo/runtime/seq.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RangeFunction = RooTest::RuntimeTestFixture;
TEST_F(RangeFunction, rising_numbers)
{
  EXPECT_EQ(runtime.eval("(range 5 10)")->to_string(), "[5 6 7 8 9 10]");
  EXPECT_EQ(runtime.eval("(range -5 2)")->to_string(), "[-5 -4 -3 -2 -1 0 1 2]");
}

TEST_F(RangeFunction, descending_numbers)
{
  EXPECT_EQ(runtime.eval("(range 10 7)")->to_string(), "[10 9 8 7]");
  EXPECT_EQ(runtime.eval("(range 2 -5)")->to_string(), "[2 1 0 -1 -2 -3 -4 -5]");
}

TEST_F(RangeFunction, float_inputs_are_truncated_to_int)
{
  // Float inputs are narrowed to int (truncation, no rounding).
  EXPECT_EQ(runtime.eval("(range 1.9 4.1)")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval("(range 1.1 4.9)")->to_string(), "[1 2 3 4]");
}

TEST_F(RangeFunction, single_arg_starts_from_zero)
{
  EXPECT_EQ(runtime.eval("(range 5)")->to_string(), "[0 1 2 3 4 5]");
  EXPECT_EQ(runtime.eval("(range 0)")->to_string(), "[0]");
}

TEST_F(RangeFunction, integer_range_elements_have_integer_type)
{
  // Integer ranges must produce INT-typed values, not floats.
  auto result = runtime.eval("(range 0 3)");

  ASSERT_EQ(Roo::count(*result), 4);
  for (const auto& el : result->elements())
  {
    const auto& num = std::get<const Roo::Value::Number>(el->value);
    EXPECT_EQ(num.num_type, Roo::Value::NumberType::INT);
  }
}

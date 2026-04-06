
#include <lisple/runtime.h>
#include <lisple/runtime/seq.h>

#include <gtest/gtest.h>

TEST(RangeFunction, rising_numbers)
{
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval("(range 5 10)")->to_string(), "[5 6 7 8 9 10]");
  EXPECT_EQ(runtime.eval("(range -5 2)")->to_string(), "[-5 -4 -3 -2 -1 0 1 2]");
}

TEST(RangeFunction, descending_numbers)
{
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval("(range 10 7)")->to_string(), "[10 9 8 7]");
  EXPECT_EQ(runtime.eval("(range 2 -5)")->to_string(), "[2 1 0 -1 -2 -3 -4 -5]");
}

TEST(RangeFunction, float_inputs_are_truncated_to_int)
{
  // Float inputs are narrowed to int (truncation, no rounding).
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval("(range 1.9 4.1)")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval("(range 1.1 4.9)")->to_string(), "[1 2 3 4]");
}

TEST(RangeFunction, single_arg_starts_from_zero)
{
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval("(range 5)")->to_string(), "[0 1 2 3 4 5]");
  EXPECT_EQ(runtime.eval("(range 0)")->to_string(), "[0]");
}

TEST(RangeFunction, integer_range_elements_have_integer_type)
{
  // Integer ranges must produce INT-typed values, not floats.
  Lisple::Runtime runtime;

  auto result = runtime.eval("(range 0 3)");

  ASSERT_EQ(Lisple::count(*result), 4);
  for (const auto& el : result->elements())
  {
    const auto& num = std::get<const Lisple::RTValue::Number>(el->value);
    EXPECT_EQ(num.num_type, Lisple::RTValue::NumberType::INT);
  }
}

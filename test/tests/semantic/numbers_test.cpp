
#include <lisple/runtime.h>
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

/*
 * ======================================================================
 * Number type semantics
 *
 * Integer inputs to arithmetic operators must produce integer outputs,
 * not silently downgrade to float. Float has only ~7 significant decimal
 * digits; using it for integer arithmetic loses precision for values
 * above 2^24 (16 777 216).
 * ======================================================================
 */

// Extract the Number struct from a result value.
static const Lisple::RTValue::Number& num_of(const Lisple::sptr_rtval& v)
{
  return std::get<const Lisple::RTValue::Number>(v->value);
}

// -----------------------------------------------------------------------
// + preserves integer type
// -----------------------------------------------------------------------

TEST(Semantic_NumberTypes, plus_int_int_produces_int)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(+ 3 4)");

  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::INT);
  EXPECT_EQ(result->i64(), 7);
}

// float can only represent ~7 significant digits exactly.
// (+ 100000001 100000001) must return 200000002, not 200000000.
TEST(Semantic_NumberTypes, plus_preserves_precision_beyond_float_range)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(+ 100000001 100000001)");

  EXPECT_EQ(result->i64(), 200000002);
}

TEST(Semantic_NumberTypes, plus_int_float_produces_float)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(+ 1 1.5)");

  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::FLOAT);
  EXPECT_DOUBLE_EQ(result->f64(), 2.5);
}

TEST(Semantic_NumberTypes, plus_float_float_produces_float)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(+ 1.5 2.5)");

  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::FLOAT);
  EXPECT_DOUBLE_EQ(result->f64(), 4.0);
}

// -----------------------------------------------------------------------
// - preserves integer type
// -----------------------------------------------------------------------

TEST(Semantic_NumberTypes, minus_int_int_produces_int)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(- 10 3)");

  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::INT);
  EXPECT_EQ(result->i64(), 7);
}

TEST(Semantic_NumberTypes, minus_preserves_precision_beyond_float_range)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(- 100000002 1)");

  EXPECT_EQ(result->i64(), 100000001);
}

// -----------------------------------------------------------------------
// / preserves integer type for exact division
// -----------------------------------------------------------------------

TEST(Semantic_NumberTypes, divide_exact_int_int_produces_int)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(/ 10 2)");

  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::INT);
  EXPECT_EQ(result->i64(), 5);
}

TEST(Semantic_NumberTypes, divide_inexact_int_int_produces_float)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(/ 10 3)");

  // 10/3 is not a whole number, result must be float
  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::FLOAT);
}

// -----------------------------------------------------------------------
// Multi-argument + stays integer if all args are integers
// -----------------------------------------------------------------------

TEST(Semantic_NumberTypes, plus_multiple_ints_stays_int)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(+ 1 2 3 4)");

  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::INT);
  EXPECT_EQ(result->i64(), 10);
}

TEST(Semantic_NumberTypes, plus_multiple_with_one_float_becomes_float)
{
  Lisple::Runtime runtime;

  auto result = runtime.eval("(+ 1 2 0.5 4)");

  EXPECT_EQ(num_of(result).num_type, Lisple::RTValue::NumberType::FLOAT);
  EXPECT_DOUBLE_EQ(result->f64(), 7.5);
}

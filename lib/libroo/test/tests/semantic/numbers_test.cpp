
#include <roo/runtime/value.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_NumberTypes = RooTest::RuntimeTestFixture;
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
static const Roo::Value::Number& num_of(const Roo::sptr_val& v)
{
  return std::get<const Roo::Value::Number>(v->value);
}

// -----------------------------------------------------------------------
// + preserves integer type
// -----------------------------------------------------------------------

TEST_F(Semantic_NumberTypes, plus_int_int_produces_int)
{
  auto result = runtime.eval("(+ 3 4)");

  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::INT);
  EXPECT_EQ(result->i64(), 7);
}

// float can only represent ~7 significant digits exactly.
// (+ 100000001 100000001) must return 200000002, not 200000000.
TEST_F(Semantic_NumberTypes, plus_preserves_precision_beyond_float_range)
{
  auto result = runtime.eval("(+ 100000001 100000001)");

  EXPECT_EQ(result->i64(), 200000002);
}

TEST_F(Semantic_NumberTypes, plus_int_float_produces_float)
{
  auto result = runtime.eval("(+ 1 1.5)");

  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::FLOAT);
  EXPECT_DOUBLE_EQ(result->f64(), 2.5);
}

TEST_F(Semantic_NumberTypes, plus_float_float_produces_float)
{
  auto result = runtime.eval("(+ 1.5 2.5)");

  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::FLOAT);
  EXPECT_DOUBLE_EQ(result->f64(), 4.0);
}

// -----------------------------------------------------------------------
// - preserves integer type
// -----------------------------------------------------------------------

TEST_F(Semantic_NumberTypes, minus_int_int_produces_int)
{
  auto result = runtime.eval("(- 10 3)");

  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::INT);
  EXPECT_EQ(result->i64(), 7);
}

TEST_F(Semantic_NumberTypes, minus_preserves_precision_beyond_float_range)
{
  auto result = runtime.eval("(- 100000002 1)");

  EXPECT_EQ(result->i64(), 100000001);
}

// -----------------------------------------------------------------------
// / preserves integer type for exact division
// -----------------------------------------------------------------------

TEST_F(Semantic_NumberTypes, divide_exact_int_int_produces_int)
{
  auto result = runtime.eval("(/ 10 2)");

  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::INT);
  EXPECT_EQ(result->i64(), 5);
}

TEST_F(Semantic_NumberTypes, divide_inexact_int_int_produces_float)
{
  auto result = runtime.eval("(/ 10 3)");

  // 10/3 is not a whole number, result must be float
  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::FLOAT);
}

// -----------------------------------------------------------------------
// Multi-argument + stays integer if all args are integers
// -----------------------------------------------------------------------

TEST_F(Semantic_NumberTypes, plus_multiple_ints_stays_int)
{
  auto result = runtime.eval("(+ 1 2 3 4)");

  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::INT);
  EXPECT_EQ(result->i64(), 10);
}

TEST_F(Semantic_NumberTypes, plus_multiple_with_one_float_becomes_float)
{
  auto result = runtime.eval("(+ 1 2 0.5 4)");

  EXPECT_EQ(num_of(result).num_type, Roo::Value::NumberType::FLOAT);
  EXPECT_DOUBLE_EQ(result->f64(), 7.5);
}

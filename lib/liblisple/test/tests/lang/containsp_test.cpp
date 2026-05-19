#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ContainsPredicateFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * ContainsPredicateFunction - (contains? [...] predicate)
 * ======================================================================
 */

TEST_F(ContainsPredicateFunction, contains_vector)
{
  // Given
  runtime.eval("(def my-vec [1 3 5 6 7 8])");

  // Then
  EXPECT_EQ(*runtime.eval("(contains? my-vec 1)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 2)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 3)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 4)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 5)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 6)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 7)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 8)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 9)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 10)"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(ContainsPredicateFunction, contains_string_as_char_sequence)
{
  EXPECT_EQ(*runtime.eval("(contains? \"abc\" 'b')"), *Lisple::Constant::BOOL_TRUE);
}

TEST_F(ContainsPredicateFunction, contains_map_as_interleaved_sequence)
{
  EXPECT_EQ(*runtime.eval("(contains? {:a 1 :b 2} :b)"), *Lisple::Constant::BOOL_TRUE);
}

TEST_F(ContainsPredicateFunction, contains_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(contains? values 2)"), *Lisple::Constant::BOOL_TRUE);
}

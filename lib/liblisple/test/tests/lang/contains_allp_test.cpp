#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ContainsAllPredicateFunction = LispleTest::RuntimeTestFixture;

TEST_F(ContainsAllPredicateFunction, contains_all_vector)
{
  runtime.eval("(def my-vec [1 3 5 6 7 8])");

  EXPECT_EQ(*runtime.eval("(contains-all? my-vec [1 5 8])"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-all? my-vec [1 5 9])"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains-all? my-vec [])"), *Lisple::Constant::BOOL_TRUE);
}

TEST_F(ContainsAllPredicateFunction, contains_all_string_as_char_sequences)
{
  EXPECT_EQ(*runtime.eval("(contains-all? \"abc\" \"ca\")"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-all? \"abc\" \"cad\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains-all? \"abc\" \"\")"), *Lisple::Constant::BOOL_TRUE);
}

TEST_F(ContainsAllPredicateFunction, contains_all_map_as_interleaved_sequence)
{
  EXPECT_EQ(*runtime.eval("(contains-all? {:a 1 :b 2} [:a 2])"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-all? {:a 1 :b 2} [:a 3])"),
            *Lisple::Constant::BOOL_FALSE);
}

TEST_F(ContainsAllPredicateFunction, contains_all_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(contains-all? values [1 3])"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-all? values [1 4])"), *Lisple::Constant::BOOL_FALSE);
}

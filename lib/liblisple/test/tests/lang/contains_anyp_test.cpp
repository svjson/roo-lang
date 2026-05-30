#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ContainsAnyPredicateFunction = LispleTest::RuntimeTestFixture;

TEST_F(ContainsAnyPredicateFunction, contains_any_vector)
{
  runtime.eval("(def my-vec [1 3 5 6 7 8])");

  EXPECT_EQ(*runtime.eval("(contains-any? my-vec [2 4 6])"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? my-vec [2 4 9])"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains-any? my-vec [])"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(ContainsAnyPredicateFunction, contains_any_string_as_char_sequences)
{
  EXPECT_EQ(*runtime.eval("(contains-any? \"abc\" \"xb\")"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? \"abc\" \"xy\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains-any? \"abc\" \"\")"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(ContainsAnyPredicateFunction, contains_any_map_as_interleaved_sequence)
{
  EXPECT_EQ(*runtime.eval("(contains-any? {:a 1 :b 2} [:missing 2])"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? {:a 1 :b 2} [:missing 3])"),
            *Lisple::Constant::BOOL_FALSE);
}

TEST_F(ContainsAnyPredicateFunction, contains_any_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(contains-any? values [0 2])"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? values [0 4])"), *Lisple::Constant::BOOL_FALSE);
}

#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ContainsAnyPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(ContainsAnyPredicateFunction, contains_any_vector)
{
  runtime.eval("(def my-vec [1 3 5 6 7 8])");

  EXPECT_EQ(*runtime.eval("(contains-any? my-vec [2 4 6])"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? my-vec [2 4 9])"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains-any? my-vec [])"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(ContainsAnyPredicateFunction, contains_any_string_as_char_sequences)
{
  EXPECT_EQ(*runtime.eval("(contains-any? \"abc\" \"xb\")"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? \"abc\" \"xy\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains-any? \"abc\" \"\")"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(ContainsAnyPredicateFunction, contains_any_map_as_interleaved_sequence)
{
  EXPECT_EQ(*runtime.eval("(contains-any? {:a 1 :b 2} [:missing 2])"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? {:a 1 :b 2} [:missing 3])"),
            *Roo::Constant::BOOL_FALSE);
}

TEST_F(ContainsAnyPredicateFunction, contains_any_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(contains-any? values [0 2])"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-any? values [0 4])"), *Roo::Constant::BOOL_FALSE);
}

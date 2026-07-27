#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SequentialPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(SequentialPredicateFunction, linear_collections_are_sequential)
{
  EXPECT_EQ(*runtime.eval("(sequential? [1 2 3])"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(sequential? '(1 2 3))"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SequentialPredicateFunction, host_sequences_are_sequential)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(sequential? values)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SequentialPredicateFunction, maps_and_strings_are_not_sequential)
{
  EXPECT_EQ(*runtime.eval("(sequential? {:a 1})"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval(R"((sequential? "abc"))"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(SequentialPredicateFunction, ordinary_values_are_not_sequential)
{
  EXPECT_EQ(*runtime.eval("(sequential? nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(sequential? 42)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(sequential? :name)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(sequential? true)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(sequential? (fn [x] x))"), *Roo::Constant::BOOL_FALSE);
}

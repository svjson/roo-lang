#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SeqPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(SeqPredicateFunction, collections_are_sequences)
{
  EXPECT_EQ(*runtime.eval("(seq? [1 2 3])"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(seq? '(1 2 3))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(seq? {:a 1})"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SeqPredicateFunction, host_sequences_are_sequences)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(seq? values)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SeqPredicateFunction, strings_are_not_sequences)
{
  EXPECT_EQ(*runtime.eval(R"((seq? "abc"))"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(SeqPredicateFunction, ordinary_values_are_not_sequences)
{
  EXPECT_EQ(*runtime.eval("(seq? nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq? 42)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq? :name)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq? true)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq? (fn [x] x))"), *Roo::Constant::BOOL_FALSE);
}

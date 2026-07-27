#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SeqablePredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(SeqablePredicateFunction, collections_and_strings_are_seqable)
{
  EXPECT_EQ(*runtime.eval("(seqable? [1 2 3])"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(seqable? '(1 2 3))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(seqable? {:a 1})"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((seqable? "abc"))"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SeqablePredicateFunction, host_sequences_are_seqable)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(seqable? values)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SeqablePredicateFunction, ordinary_values_are_not_seqable)
{
  EXPECT_EQ(*runtime.eval("(seqable? nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seqable? 42)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seqable? :name)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seqable? true)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seqable? (fn [x] x))"), *Roo::Constant::BOOL_FALSE);
}

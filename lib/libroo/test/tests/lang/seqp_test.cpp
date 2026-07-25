#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SeqPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(SeqPredicateFunction, sequence_values_are_sequences)
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

using SeqLikePredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(SeqLikePredicateFunction, sequences_and_strings_are_seq_like)
{
  EXPECT_EQ(*runtime.eval("(seq-like? [1 2 3])"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(seq-like? '(1 2 3))"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(seq-like? {:a 1})"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((seq-like? "abc"))"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SeqLikePredicateFunction, host_sequences_are_seq_like)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(seq-like? values)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(SeqLikePredicateFunction, ordinary_values_are_not_seq_like)
{
  EXPECT_EQ(*runtime.eval("(seq-like? nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq-like? 42)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq-like? :name)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq-like? true)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(seq-like? (fn [x] x))"), *Roo::Constant::BOOL_FALSE);
}

#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using EveryFunction = RooTest::RuntimeTestFixture;

TEST_F(EveryFunction, returns_true_when_every_element_matches)
{
  EXPECT_EQ(*runtime.eval("(every? [2 4 6] even?)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EveryFunction, returns_false_when_an_element_does_not_match)
{
  EXPECT_EQ(*runtime.eval("(every? [2 3 4] even?)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(EveryFunction, returns_true_for_empty_sequence)
{
  EXPECT_EQ(*runtime.eval("(every? [] even?)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EveryFunction, uses_truthiness_of_predicate_results)
{
  EXPECT_EQ(*runtime.eval("(every? [1 :value] identity)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(every? [1 nil] identity)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(EveryFunction, stops_after_first_falsey_result)
{
  runtime.eval("(def eval-count 0)");
  runtime.eval(
    "(defun counted-even? [value] (set! [eval-count] (inc eval-count)) (even? value))");

  EXPECT_EQ(*runtime.eval("(every? [2 3 4] counted-even?)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(runtime.eval("eval-count")->i64(), 2);
}

TEST_F(EveryFunction, traverses_string_as_character_sequence)
{
  EXPECT_EQ(*runtime.eval(R"((every? "aaa" (fn [character] (= character 'a'))))"),
            *Roo::Constant::BOOL_TRUE);
}

TEST_F(EveryFunction, traverses_map_as_interleaved_sequence)
{
  EXPECT_EQ(*runtime.eval("(every? {:a 1 :b 2} identity)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EveryFunction, traverses_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {2, 4, 6};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(every? values even?)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EveryFunction, accepts_predicate_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(every? even? [2 4 6])"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EveryFunction, accepts_keyword_callable)
{
  EXPECT_EQ(*runtime.eval("(every? [{:active true} {:active 1}] :active)"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(every? :active [{:active true} {}])"),
            *Roo::Constant::BOOL_FALSE);
}

TEST_F(EveryFunction, treats_nil_as_empty_sequence_in_sequence_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(every? nil even?)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EveryFunction, treats_nil_as_empty_sequence_in_predicate_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(every? even? nil)"), *Roo::Constant::BOOL_TRUE);
}

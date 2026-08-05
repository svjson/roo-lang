#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FindIndexFunction = RooTest::RuntimeTestFixture;

TEST_F(FindIndexFunction, finds_index_in_vector)
{
  EXPECT_EQ(runtime.eval("(find-index [1 2 3] even?)")->to_string(), "1");
}

TEST_F(FindIndexFunction, finds_index_in_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((find-index "abc" (fn [c] (= c 'b'))))")->to_string(), "1");
}

TEST_F(FindIndexFunction, finds_index_in_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(find-index {:a 1 :b 2} (fn [x] (= x :b)))")->to_string(), "2");
}

TEST_F(FindIndexFunction, finds_index_in_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(find-index values even?)")->to_string(), "1");
}

TEST_F(FindIndexFunction, accepts_predicate_first_arg_order)
{
  EXPECT_EQ(runtime.eval("(find-index even? [1 3 4])")->to_string(), "2");
}

TEST_F(FindIndexFunction, treats_nil_as_sequence_in_sequence_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(find-index nil even?)"), *Roo::Constant::NIL);
}

TEST_F(FindIndexFunction, treats_nil_as_sequence_in_predicate_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(find-index even? nil)"), *Roo::Constant::NIL);
}

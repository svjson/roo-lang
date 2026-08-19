#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MoveNthFunction = RooTest::RuntimeTestFixture;

TEST_F(MoveNthFunction, moves_element_to_final_index)
{
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 1 3)")->to_string(), "[:a :c :d :b]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 3 1)")->to_string(), "[:a :d :b :c]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 1 :final 3)")->to_string(),
            "[:a :c :d :b]");
}

TEST_F(MoveNthFunction, moves_element_relative_to_target_in_input_order)
{
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 1 :before 3)")->to_string(),
            "[:a :c :b :d]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 3 :before 1)")->to_string(),
            "[:a :d :b :c]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 1 :after 2)")->to_string(),
            "[:a :c :b :d]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 3 :after 1)")->to_string(),
            "[:a :b :d :c]");
}

TEST_F(MoveNthFunction, moving_relative_to_itself_is_a_no_op)
{
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c] 1 :before 1)")->to_string(), "[:a :b :c]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c] 1 :after 1)")->to_string(), "[:a :b :c]");
}

TEST_F(MoveNthFunction, resolves_negative_indexes_from_end)
{
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] -1 0)")->to_string(), "[:d :a :b :c]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c :d] 0 -1)")->to_string(), "[:b :c :d :a]");
}

TEST_F(MoveNthFunction, clamps_indexes_to_sequence_bounds)
{
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c] 0 99)")->to_string(), "[:b :c :a]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c] -99 1)")->to_string(), "[:b :a :c]");
  EXPECT_EQ(runtime.eval("(move-nth [:a :b :c] 99 0)")->to_string(), "[:a :b :c]");
}

TEST_F(MoveNthFunction, treats_nil_as_an_empty_sequence)
{
  EXPECT_EQ(runtime.eval("(move-nth nil 0 0)")->to_string(), "[]");
}

TEST_F(MoveNthFunction, accepts_lists_and_native_host_sequences)
{
  EXPECT_EQ(runtime.eval("(move-nth '(:a :b :c) 0 2)")->to_string(), "[:b :c :a]");

  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));
  EXPECT_EQ(runtime.eval("(move-nth values 0 2)")->to_string(), "[2 3 1]");
  EXPECT_EQ(values, (std::vector<int>{1, 2, 3}));
}

TEST_F(MoveNthFunction, does_not_mutate_the_original_sequence)
{
  runtime.eval("(def original [:a :b :c])");

  EXPECT_EQ(runtime.eval("(move-nth original 0 2)")->to_string(), "[:b :c :a]");
  EXPECT_EQ(runtime.lookup("original")->to_string(), "[:a :b :c]");
}

TEST_F(MoveNthFunction, rejects_fractional_indexes)
{
  EXPECT_THROW(runtime.eval("(move-nth [:a :b] 0.5 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(move-nth [:a :b] 0 1.5)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(move-nth nil 0.5 1)"), Roo::TypeError);
}

TEST_F(MoveNthFunction, rejects_unknown_placements)
{
  EXPECT_THROW(runtime.eval("(move-nth [:a :b] 0 :near 1)"), Roo::InvocationException);
}

TEST_F(MoveNthFunction, rejects_maps_and_strings)
{
  EXPECT_THROW(runtime.eval("(move-nth {:a 1} 0 0)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((move-nth "abc" 0 1))"), Roo::InvocationException);
}

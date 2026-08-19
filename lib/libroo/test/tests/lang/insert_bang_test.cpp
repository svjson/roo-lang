#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using InsertBangFunction = RooTest::RuntimeTestFixture;

TEST_F(InsertBangFunction, inserts_and_returns_the_mutated_vector)
{
  runtime.eval("(def target [1 4])");

  EXPECT_EQ(runtime.eval("(insert! target 1 [2 3])")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.lookup("target")->to_string(), "[1 2 3 4]");
}

TEST_F(InsertBangFunction, mutates_lists_and_preserves_their_type)
{
  runtime.eval("(def target '(1 4))");

  EXPECT_EQ(runtime.eval("(insert! target 1 '(2 3))")->to_string(), "(1 2 3 4)");
  EXPECT_EQ(runtime.lookup("target")->to_string(), "(1 2 3 4)");
}

TEST_F(InsertBangFunction, snapshots_values_before_mutating_the_target)
{
  runtime.eval("(def target [1 2])");

  EXPECT_EQ(runtime.eval("(insert! target 1 target)")->to_string(), "[1 1 2 2]");
}

TEST_F(InsertBangFunction, uses_clamped_end_relative_boundaries)
{
  runtime.eval("(def target [1 4])");
  runtime.eval("(insert! target -1 [2 3])");
  runtime.eval("(insert! target 99 [5 6])");
  runtime.eval("(insert! target -99 [-1 0])");

  EXPECT_EQ(runtime.lookup("target")->to_string(), "[-1 0 1 2 3 4 5 6]");
}

TEST_F(InsertBangFunction, treats_nil_as_an_empty_sequence)
{
  runtime.eval("(def target nil)");

  EXPECT_EQ(runtime.eval("(insert! target 20 [1 2])")->to_string(), "[1 2]");
  EXPECT_EQ(*runtime.lookup("target"), *Roo::Constant::NIL);
}

TEST_F(InsertBangFunction, mutates_native_host_sequences)
{
  std::vector<int> target = {1, 4};
  std::vector<int> values = {2, 3};
  runtime.get_current_namespace().store("target",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(target));
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(insert! target 1 values)")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(target, (std::vector<int>{1, 2, 3, 4}));
}

TEST_F(InsertBangFunction, requires_an_integer_index_and_sequential_values)
{
  EXPECT_THROW(runtime.eval("(insert! [1 2] 0.5 [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(insert! [1 2] 1 3)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((insert! [1 2] 1 "xy"))"), Roo::InvocationException);
}

TEST_F(InsertBangFunction, rejects_map_and_string_targets)
{
  EXPECT_THROW(runtime.eval("(insert! {:a 1} 0 [2])"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((insert! "abc" 0 ["x"]))"), Roo::InvocationException);
}

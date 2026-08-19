#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ReplaceBangFunction = RooTest::RuntimeTestFixture;

TEST_F(ReplaceBangFunction, replaces_and_returns_the_mutated_vector)
{
  runtime.eval("(def target [1 2 3 4 5])");

  EXPECT_EQ(runtime.eval("(replace! target 2 4 [:a :b :c])")->to_string(),
            "[1 2 :a :b :c 5]");
  EXPECT_EQ(runtime.lookup("target")->to_string(), "[1 2 :a :b :c 5]");
}

TEST_F(ReplaceBangFunction, mutates_lists_and_preserves_their_type)
{
  runtime.eval("(def target '(1 2 3 4))");

  EXPECT_EQ(runtime.eval("(replace! target 1 3 '(:a :b))")->to_string(), "(1 :a :b 4)");
}

TEST_F(ReplaceBangFunction, snapshots_values_before_mutating_the_target)
{
  runtime.eval("(def target [1 2 3 4])");

  EXPECT_EQ(runtime.eval("(replace! target 1 3 target)")->to_string(), "[1 1 2 3 4 4]");
}

TEST_F(ReplaceBangFunction, treats_empty_ranges_as_insertion)
{
  runtime.eval("(def target [1 4])");

  EXPECT_EQ(runtime.eval("(replace! target 1 1 [2 3])")->to_string(), "[1 2 3 4]");
}

TEST_F(ReplaceBangFunction, treats_nil_as_an_empty_sequence)
{
  runtime.eval("(def target nil)");

  EXPECT_EQ(runtime.eval("(replace! target 0 0 [1 2])")->to_string(), "[1 2]");
  EXPECT_EQ(*runtime.lookup("target"), *Roo::Constant::NIL);
}

TEST_F(ReplaceBangFunction, mutates_native_host_sequences)
{
  std::vector<int> target = {1, 2, 3, 4};
  std::vector<int> values = {8, 9, 10};
  runtime.get_current_namespace().store("target",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(target));
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(replace! target 1 3 values)")->to_string(), "[1 8 9 10 4]");
  EXPECT_EQ(target, (std::vector<int>{1, 8, 9, 10, 4}));
}

TEST_F(ReplaceBangFunction, validates_arguments_and_rejects_immutable_targets)
{
  EXPECT_THROW(runtime.eval("(replace! [1 2] nil 1 [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(replace! [1 2] 0 nil [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(replace! [1 2] 0.5 1 [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(replace! [1 2] 0 1.5 [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(replace! [1 2] 0 1 3)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(replace! {:a 1} 0 1 [2])"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((replace! "abc" 0 1 ["x"]))"), Roo::InvocationException);
}

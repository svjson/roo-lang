#include <vector>

#include <roo/host/std_adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MoveNthBangFunction = RooTest::RuntimeTestFixture;

TEST_F(MoveNthBangFunction, moves_and_returns_the_mutated_sequence)
{
  runtime.eval("(def values [:a :b :c :d])");
  Roo::sptr_val original = runtime.lookup("values");
  Roo::sptr_val result = runtime.eval("(move-nth! values 1 3)");

  EXPECT_EQ(result.get(), original.get());
  EXPECT_EQ(result->to_string(), "[:a :c :d :b]");
  EXPECT_EQ(runtime.lookup("values")->to_string(), "[:a :c :d :b]");
}

TEST_F(MoveNthBangFunction, supports_explicit_placement_modes)
{
  runtime.eval("(def before-values [:a :b :c :d])");
  runtime.eval("(def after-values [:a :b :c :d])");
  runtime.eval("(def final-values [:a :b :c :d])");

  EXPECT_EQ(runtime.eval("(move-nth! before-values 1 :before 3)")->to_string(),
            "[:a :c :b :d]");
  EXPECT_EQ(runtime.eval("(move-nth! after-values 3 :after 1)")->to_string(),
            "[:a :b :d :c]");
  EXPECT_EQ(runtime.eval("(move-nth! final-values 1 :final 3)")->to_string(),
            "[:a :c :d :b]");
}

TEST_F(MoveNthBangFunction, mutates_lists)
{
  runtime.eval("(def values '(:a :b :c))");

  EXPECT_EQ(runtime.eval("(move-nth! values 0 2)")->to_string(), "(:b :c :a)");
  EXPECT_EQ(runtime.lookup("values")->to_string(), "(:b :c :a)");
}

TEST_F(MoveNthBangFunction, treats_nil_as_an_empty_sequence)
{
  runtime.eval("(def values nil)");

  EXPECT_EQ(runtime.eval("(move-nth! values 0 0)")->to_string(), "[]");
  EXPECT_EQ(*runtime.lookup("values"), *Roo::Constant::NIL);
}

TEST_F(MoveNthBangFunction, resolves_and_clamps_indexes)
{
  runtime.eval("(def values [:a :b :c])");

  EXPECT_EQ(runtime.eval("(move-nth! values -1 -99)")->to_string(), "[:c :a :b]");
  EXPECT_EQ(runtime.eval("(move-nth! values 99 0)")->to_string(), "[:c :a :b]");
}

TEST_F(MoveNthBangFunction, rejects_invalid_arguments)
{
  EXPECT_THROW(runtime.eval("(move-nth! [:a :b] nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(move-nth! [:a :b] 0 nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(move-nth! [:a :b] 0.5 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(move-nth! nil 0.5 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(move-nth! [:a :b] 0 :near 1)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(move-nth! {:a 1} 0 0)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((move-nth! "abc" 0 1))"), Roo::InvocationException);
}

TEST_F(MoveNthBangFunction, mutates_native_host_sequences)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(move-nth! values 0 2)")->to_string(), "[2 3 1]");
  EXPECT_EQ(values, (std::vector<int>{2, 3, 1}));
}

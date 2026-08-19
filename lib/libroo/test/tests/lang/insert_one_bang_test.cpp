#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using InsertOneBangFunction = RooTest::RuntimeTestFixture;

TEST_F(InsertOneBangFunction, inserts_and_returns_the_mutated_sequence)
{
  runtime.eval("(def values [1 3])");

  EXPECT_EQ(runtime.eval("(insert-one! values 1 2)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.lookup("values")->to_string(), "[1 2 3]");
}

TEST_F(InsertOneBangFunction, uses_clamped_end_relative_boundaries)
{
  runtime.eval("(def values [1 2 3])");
  runtime.eval("(insert-one! values -1 :before-last)");
  runtime.eval("(insert-one! values 99 :last)");
  runtime.eval("(insert-one! values -99 :first)");

  EXPECT_EQ(runtime.lookup("values")->to_string(), "[:first 1 2 :before-last 3 :last]");
}

TEST_F(InsertOneBangFunction, inserts_sequential_values_as_one_element)
{
  runtime.eval("(def values [1 3])");

  EXPECT_EQ(runtime.eval("(insert-one! values 1 [:a :b])")->to_string(), "[1 [:a :b] 3]");
}

TEST_F(InsertOneBangFunction, mutates_lists)
{
  runtime.eval("(def values '(1 3))");

  EXPECT_EQ(runtime.eval("(insert-one! values 1 2)")->to_string(), "(1 2 3)");
  EXPECT_EQ(runtime.lookup("values")->to_string(), "(1 2 3)");
}

TEST_F(InsertOneBangFunction, treats_nil_as_an_empty_sequence)
{
  runtime.eval("(def values nil)");

  EXPECT_EQ(runtime.eval("(insert-one! values 20 :x)")->to_string(), "[:x]");
  EXPECT_EQ(*runtime.lookup("values"), *Roo::Constant::NIL);
}

TEST_F(InsertOneBangFunction, rejects_fractional_indexes)
{
  EXPECT_THROW(runtime.eval("(insert-one! [1 2] 0.5 :x)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(insert-one! nil 0.5 :x)"), Roo::TypeError);
}

TEST_F(InsertOneBangFunction, rejects_maps_and_strings)
{
  EXPECT_THROW(runtime.eval("(insert-one! {:a 1} 0 :x)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((insert-one! "abc" 0 :x))"), Roo::InvocationException);
}

TEST_F(InsertOneBangFunction, mutates_native_host_sequences)
{
  std::vector<int> values = {1, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(insert-one! values 1 2)")->to_string(), "[1 2 3]");
  EXPECT_EQ(values, (std::vector<int>{1, 2, 3}));
}

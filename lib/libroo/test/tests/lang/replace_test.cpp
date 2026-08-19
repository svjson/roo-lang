#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ReplaceFunction = RooTest::RuntimeTestFixture;

TEST_F(ReplaceFunction, replaces_a_half_open_sequence_range)
{
  EXPECT_EQ(runtime.eval("(replace [1 2 3 4 5] 2 4 [:a :b :c])")->to_string(),
            "[1 2 :a :b :c 5]");
}

TEST_F(ReplaceFunction, expands_sequential_replacement_values)
{
  EXPECT_EQ(runtime.eval("(replace [1 2 3 4] 1 3 [[:a :b]])")->to_string(), "[1 [:a :b] 4]");
  EXPECT_EQ(runtime.eval("(replace '(1 2 3 4) 1 3 '(:a :b))")->to_string(), "[1 :a :b 4]");
}

TEST_F(ReplaceFunction, composes_as_insert_and_drop_for_empty_ranges_or_values)
{
  EXPECT_EQ(runtime.eval("(replace [1 4] 1 1 [2 3])")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval("(replace [1 2 3 4] 1 3 [])")->to_string(), "[1 4]");
  EXPECT_EQ(runtime.eval("(replace [1 4] 2 1 [2 3])")->to_string(), "[1 4 2 3]");
}

TEST_F(ReplaceFunction, resolves_and_clamps_end_relative_boundaries)
{
  EXPECT_EQ(runtime.eval("(replace [1 2 3 4 5] -3 -1 [:a])")->to_string(), "[1 2 :a 5]");
  EXPECT_EQ(runtime.eval("(replace [1 2 3] -20 20 [:a])")->to_string(), "[:a]");
  EXPECT_EQ(runtime.eval("(replace [1 2 3] 20 40 [:a])")->to_string(), "[1 2 3 :a]");
}

TEST_F(ReplaceFunction, replaces_string_ranges_using_insert_conversion_rules)
{
  EXPECT_EQ(runtime.eval(R"((replace "abcdef" 2 4 "XYZ"))")->to_string(), R"("abXYZef")");
  EXPECT_EQ(runtime.eval(R"((replace "abcdef" 2 4 [8 true :x nil]))")->to_string(),
            R"("ab8true:xnilef")");
  EXPECT_EQ(runtime.eval(R"((replace "abcdef" 2 4 'x'))")->to_string(), R"("abxef")");
  EXPECT_EQ(runtime.eval(R"((replace "abcdef" 2 4 {:x 1}))")->to_string(),
            R"("ab{:x 1}ef")");
}

TEST_F(ReplaceFunction, uses_string_ranges_without_mutating_the_original)
{
  runtime.eval(R"((def original "abcdef"))");

  EXPECT_EQ(runtime.eval(R"((replace original -3 -1 ["X"]))")->to_string(), R"("abcXf")");
  EXPECT_EQ(runtime.lookup("original")->to_string(), R"("abcdef")");
}

TEST_F(ReplaceFunction, accepts_nil_and_native_host_sequences)
{
  EXPECT_EQ(runtime.eval("(replace nil 0 0 [1 2])")->to_string(), "[1 2]");

  std::vector<int> target = {1, 2, 3, 4};
  runtime.get_current_namespace().store("target",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(target));

  EXPECT_EQ(runtime.eval("(replace target 1 3 [8 9])")->to_string(), "[1 8 9 4]");
  EXPECT_EQ(target, (std::vector<int>{1, 2, 3, 4}));
}

TEST_F(ReplaceFunction, requires_integer_boundaries_and_sequence_values_for_sequences)
{
  EXPECT_THROW(runtime.eval("(replace [1 2] 0.5 1 [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(replace [1 2] 0 1.5 [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(replace [1 2] 0 1 3)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((replace [1 2] 0 1 "xy"))"), Roo::InvocationException);
}

TEST_F(ReplaceFunction, rejects_map_targets)
{
  EXPECT_THROW(runtime.eval("(replace {:a 1} 0 1 [2])"), Roo::InvocationException);
}

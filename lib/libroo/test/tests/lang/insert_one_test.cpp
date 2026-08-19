#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using InsertOneFunction = RooTest::RuntimeTestFixture;

TEST_F(InsertOneFunction, inserts_at_front_middle_and_end)
{
  EXPECT_EQ(runtime.eval("(insert-one [2 3] 0 1)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(insert-one [1 3] 1 2)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(insert-one [1 2] 2 3)")->to_string(), "[1 2 3]");
}

TEST_F(InsertOneFunction, resolves_negative_indexes_from_end)
{
  EXPECT_EQ(runtime.eval("(insert-one [1 2 3] -1 :x)")->to_string(), "[1 2 :x 3]");
  EXPECT_EQ(runtime.eval("(insert-one [1 2 3] -2 :x)")->to_string(), "[1 :x 2 3]");
}

TEST_F(InsertOneFunction, clamps_indexes_to_sequence_boundaries)
{
  EXPECT_EQ(runtime.eval("(insert-one [1 2] 20 :x)")->to_string(), "[1 2 :x]");
  EXPECT_EQ(runtime.eval("(insert-one [1 2] -20 :x)")->to_string(), "[:x 1 2]");
}

TEST_F(InsertOneFunction, inserts_sequential_values_as_one_element)
{
  EXPECT_EQ(runtime.eval("(insert-one [1 3] 1 [:a :b])")->to_string(), "[1 [:a :b] 3]");
}

TEST_F(InsertOneFunction, inserts_strings_and_characters_into_strings)
{
  EXPECT_EQ(runtime.eval(R"((insert-one "abcd" 2 "."))")->to_string(), R"("ab.cd")");
  EXPECT_EQ(runtime.eval(R"((insert-one "abcd" 2 'x'))")->to_string(), R"("abxcd")");
}

TEST_F(InsertOneFunction, stringifies_one_complete_value_for_string_targets)
{
  EXPECT_EQ(runtime.eval(R"((insert-one "abcd" 2 8))")->to_string(), R"("ab8cd")");
  EXPECT_EQ(runtime.eval(R"((insert-one "abcd" 2 true))")->to_string(), R"("abtruecd")");
  EXPECT_EQ(runtime.eval(R"((insert-one "abcd" 2 nil))")->to_string(), R"("abnilcd")");
  EXPECT_EQ(runtime.eval(R"((insert-one "abcd" 2 ["." "!"]))")->to_string(),
            R"("ab[\".\" \"!\"]cd")");
  EXPECT_EQ(runtime.eval(R"((insert-one "abcd" 2 {:x 1}))")->to_string(), R"("ab{:x 1}cd")");
}

TEST_F(InsertOneFunction, uses_string_boundaries_without_mutating_the_original)
{
  runtime.eval(R"((def original "abcd"))");

  EXPECT_EQ(runtime.eval(R"((insert-one original -1 "."))")->to_string(), R"("abc.d")");
  EXPECT_EQ(runtime.eval(R"((insert-one original 99 "!"))")->to_string(), R"("abcd!")");
  EXPECT_EQ(runtime.eval(R"((insert-one original -99 "!"))")->to_string(), R"("!abcd")");
  EXPECT_EQ(runtime.lookup("original")->to_string(), R"("abcd")");
}

TEST_F(InsertOneFunction, treats_nil_as_an_empty_sequence)
{
  EXPECT_EQ(runtime.eval("(insert-one nil 8 :x)")->to_string(), "[:x]");
}

TEST_F(InsertOneFunction, accepts_lists_and_native_host_sequences)
{
  EXPECT_EQ(runtime.eval("(insert-one '(1 3) 1 2)")->to_string(), "[1 2 3]");

  std::vector<int> values = {1, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));
  EXPECT_EQ(runtime.eval("(insert-one values 1 2)")->to_string(), "[1 2 3]");
  EXPECT_EQ(values, (std::vector<int>{1, 3}));
}

TEST_F(InsertOneFunction, does_not_mutate_the_original_sequence)
{
  runtime.eval("(def original [1 3])");

  EXPECT_EQ(runtime.eval("(insert-one original 1 2)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.lookup("original")->to_string(), "[1 3]");
}

TEST_F(InsertOneFunction, rejects_fractional_indexes)
{
  EXPECT_THROW(runtime.eval("(insert-one [1 2] 0.5 :x)"), Roo::TypeError);
}

TEST_F(InsertOneFunction, rejects_map_targets)
{
  EXPECT_THROW(runtime.eval("(insert-one {:a 1} 0 :x)"), Roo::InvocationException);
}

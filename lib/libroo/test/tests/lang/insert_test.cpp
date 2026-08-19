#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using InsertFunction = RooTest::RuntimeTestFixture;

TEST_F(InsertFunction, inserts_values_at_front_middle_and_end)
{
  EXPECT_EQ(runtime.eval("(insert [3 4] 0 [1 2])")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval("(insert [1 4] 1 [2 3])")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval("(insert [1 2] 2 [3 4])")->to_string(), "[1 2 3 4]");
}

TEST_F(InsertFunction, expands_values_instead_of_inserting_them_as_one_element)
{
  EXPECT_EQ(runtime.eval("(insert [1 4] 1 [2 3])")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval("(insert [1 4] 1 '([2] [3]))")->to_string(), "[1 [2] [3] 4]");
}

TEST_F(InsertFunction, inserts_text_fragments_into_strings)
{
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 "."))")->to_string(), R"("ab.cd")");
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 'x'))")->to_string(), R"("abxcd")");
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 ["." "!"]))")->to_string(), R"("ab.!cd")");
}

TEST_F(InsertFunction, stringifies_each_inserted_value_for_string_targets)
{
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 2))")->to_string(), R"("ab2cd")");
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 true))")->to_string(), R"("abtruecd")");
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 nil))")->to_string(), R"("abnilcd")");
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 {:a 1}))")->to_string(), R"("ab{:a 1}cd")");
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 [8 true :x nil]))")->to_string(),
            R"("ab8true:xnilcd")");
  EXPECT_EQ(runtime.eval(R"((insert "abcd" 2 [["." "!"]]))")->to_string(),
            R"("ab[\".\" \"!\"]cd")");
}

TEST_F(InsertFunction, uses_string_boundaries_without_mutating_the_original)
{
  runtime.eval(R"((def original "abcd"))");

  EXPECT_EQ(runtime.eval(R"((insert original -1 ["."]))")->to_string(), R"("abc.d")");
  EXPECT_EQ(runtime.eval(R"((insert original 99 ["!"]))")->to_string(), R"("abcd!")");
  EXPECT_EQ(runtime.eval(R"((insert original -99 ["!"]))")->to_string(), R"("!abcd")");
  EXPECT_EQ(runtime.lookup("original")->to_string(), R"("abcd")");
}

TEST_F(InsertFunction, resolves_and_clamps_indexes)
{
  EXPECT_EQ(runtime.eval("(insert [1 2 3] -1 [:x :y])")->to_string(), "[1 2 :x :y 3]");
  EXPECT_EQ(runtime.eval("(insert [1 2] 20 [:x])")->to_string(), "[1 2 :x]");
  EXPECT_EQ(runtime.eval("(insert [1 2] -20 [:x])")->to_string(), "[:x 1 2]");
}

TEST_F(InsertFunction, accepts_lists_nil_and_native_host_sequences)
{
  EXPECT_EQ(runtime.eval("(insert '(1 4) 1 '(2 3))")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.eval("(insert nil 8 [1 2])")->to_string(), "[1 2]");
  EXPECT_EQ(runtime.eval("(insert [1 2] 1 nil)")->to_string(), "[1 2]");

  std::vector<int> target = {1, 4};
  std::vector<int> values = {2, 3};
  runtime.get_current_namespace().store("target",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(target));
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));
  EXPECT_EQ(runtime.eval("(insert target 1 values)")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(target, (std::vector<int>{1, 4}));
}

TEST_F(InsertFunction, returns_nil_for_a_nil_target_and_scalar_value)
{
  EXPECT_EQ(runtime.eval("(insert nil 2 2)"), Roo::Constant::NIL);
}

TEST_F(InsertFunction, does_not_mutate_the_original_sequence)
{
  runtime.eval("(def original [1 4])");

  EXPECT_EQ(runtime.eval("(insert original 1 [2 3])")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(runtime.lookup("original")->to_string(), "[1 4]");
}

TEST_F(InsertFunction, requires_an_integer_index_and_sequential_values)
{
  EXPECT_THROW(runtime.eval("(insert [1 2] 0.5 [3])"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(insert [1 2] 1 3)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval(R"((insert [1 2] 1 "xy"))"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(insert [1 2] 1 {:a 1})"), Roo::InvocationException);
}

TEST_F(InsertFunction, rejects_map_targets)
{
  EXPECT_THROW(runtime.eval("(insert {:a 1} 0 [2])"), Roo::InvocationException);
}

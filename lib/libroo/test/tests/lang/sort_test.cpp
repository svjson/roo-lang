
#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SortFunction = RooTest::RuntimeTestFixture;
TEST_F(SortFunction, sort_int_vector_with_function_symbol)
{
  // Given
  EXPECT_EQ(runtime.eval("(sort [10 7 9 4 1 20] <)")->to_string(), "[1 4 7 9 10 20]");
  EXPECT_EQ(runtime.eval("(sort [10 7 9 4 1 20] >)")->to_string(), "[20 10 9 7 4 1]");
}
TEST_F(SortFunction, sort_string_vector_with_function_symbol)
{
  // Given
  EXPECT_EQ(runtime.eval(R"((sort ["Zorro" "Kenny" "Sam" "Bev"] <))")->to_string(),
            R"(["Bev" "Kenny" "Sam" "Zorro"])");
  EXPECT_EQ(runtime.eval(R"((sort ["Zorro" "Kenny" "Sam" "Bev"] >))")->to_string(),
            R"(["Zorro" "Sam" "Kenny" "Bev"])");
}

TEST_F(SortFunction, sort_map_vector_by_string_value_with_custom_function)
{
  // Given
  EXPECT_EQ(
    runtime
      .eval(R"(
(sort
  [{:name "Zorro" :age 32}
   {:name "Kenny" :age 74}
   {:name "Sam" :age 29}
   {:name "Bev" :age 16}]
  (fn [a b] (< (:name a) (:name b))))

)")
      ->to_string(),
    R"([{:name "Bev" :age 16} {:name "Kenny" :age 74} {:name "Sam" :age 29} {:name "Zorro" :age 32}])");

  EXPECT_EQ(
    runtime
      .eval(R"(
(sort
  [{:name "Zorro" :age 32}
   {:name "Kenny" :age 74}
   {:name "Sam" :age 29}
   {:name "Bev" :age 16}]
  (fn [a b] (> (:name a) (:name b))))

)")
      ->to_string(),
    R"([{:name "Zorro" :age 32} {:name "Sam" :age 29} {:name "Kenny" :age 74} {:name "Bev" :age 16}])");
}

TEST_F(SortFunction, sorts_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((sort "cba" (fn [a b] (< (str a) (str b)))))")->to_string(),
            "['a' 'b' 'c']");
}

TEST_F(SortFunction, sorts_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(sort {:b 2 :a 1} (fn [a b] (< (str a) (str b))))")->to_string(),
            "[1 2 :a :b]");
}

TEST_F(SortFunction, sorts_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(sort values >)")->to_string(), "[3 2 1]");
}

TEST_F(SortFunction, accepts_function_first_arg_order)
{
  EXPECT_EQ(runtime.eval("(sort > [1 4 2])")->to_string(), "[4 2 1]");
}

TEST_F(SortFunction, treats_nil_as_sequence_in_sequence_first_arg_order)
{
  EXPECT_EQ(runtime.eval("(sort nil <)")->to_string(), "[]");
}

TEST_F(SortFunction, treats_nil_as_sequence_in_function_first_arg_order)
{
  EXPECT_EQ(runtime.eval("(sort < nil)")->to_string(), "[]");
}


#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(SortFunction, sort_int_vector_with_function_symbol)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval("(sort [10 7 9 4 1 20] <)")->to_string(), "[1 4 7 9 10 20]");
  EXPECT_EQ(runtime.eval("(sort [10 7 9 4 1 20] >)")->to_string(), "[20 10 9 7 4 1]");
}
TEST(SortFunction, sort_string_vector_with_function_symbol)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval(R"((sort ["Zorro" "Kenny" "Sam" "Bev"] <))")->to_string(),
            R"(["Bev" "Kenny" "Sam" "Zorro"])");
  EXPECT_EQ(runtime.eval(R"((sort ["Zorro" "Kenny" "Sam" "Bev"] >))")->to_string(),
            R"(["Zorro" "Sam" "Kenny" "Bev"])");
}

TEST(SortFunction, sort_map_vector_by_string_value_with_custom_function)
{
  // Given
  Lisple::Runtime runtime;

  // When
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

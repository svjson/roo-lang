
#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

// ---------------------------------------------------------------------------
// Benchmark_Str -- exercises (str ...) with various argument types
// ---------------------------------------------------------------------------

TEST(Benchmark_Str, stringify_strings_only__dotimes_1_000_000)
{
  LispleTest::SnippetBenchmark bm(
    R"((dotimes [n 1000000] (str "hello" ", " "world" "! " "foo" " " "bar" " " "baz")))");

  bm.run();
}

TEST(Benchmark_Str, stringify_strings_and_primitives__dotimes_1_000_000)
{
  LispleTest::SnippetBenchmark bm(
    R"((dotimes [n 1000000] (str "n=" n " doubled=" (* n 2) " even=" (= (mod n 2) 0))))");

  bm.run();
}

TEST(Benchmark_Str, stringify_strings_and_complex_objects__dotimes_1_000_000)
{
  LispleTest::SnippetBenchmark bm(
    R"((let [v [1 2 3 4 5]
             m {:name "Alice" :age 30 :active true :scores [10 20 30]}]
         (dotimes [n 1000000]
           (str "v=" v " m=" m " n=" n))))");

  bm.run();
}

// ---------------------------------------------------------------------------
// Pre-built large-input variants
// ---------------------------------------------------------------------------

const std::vector<std::string> STR_PARTS_SETUP = {"(def parts-integers (range 0 1000))",

                                                  R"((def parts-strings
       (map (range 0 1000) (fn [i] (str "item-" i " ")))))",

                                                  R"((def parts-mixed
       (map (range 0 1000)
            (fn [i]
              (case (mod i 5)
                0 (str "item-" i)
                1 i
                2 (* i 0.1)
                3 [i (+ i 1) (+ i 2)]
                :default {:idx i :val (* i 2)})))))"};

TEST(Benchmark_Str, stringify_apply_1000_integers__dotimes_100_000)
{
  LispleTest::SnippetBenchmark bm(STR_PARTS_SETUP,
                                  "user",
                                  "(dotimes [n 100000] (apply str parts-integers))");
  bm.run();
}

TEST(Benchmark_Str, stringify_apply_1000_strings__dotimes_100_000)
{
  LispleTest::SnippetBenchmark bm(STR_PARTS_SETUP,
                                  "user",
                                  "(dotimes [n 100000] (apply str parts-strings))");
  bm.run();
}

TEST(Benchmark_Str, stringify_apply_1000_mixed__dotimes_10_000)
{
  LispleTest::SnippetBenchmark bm(STR_PARTS_SETUP,
                                  "user",
                                  "(dotimes [n 10000] (apply str parts-mixed))");
  bm.run();
}

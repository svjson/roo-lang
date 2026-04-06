
#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

// ---------------------------------------------------------------------------
// Benchmark_Str -- exercises (str ...) with various argument types
// ---------------------------------------------------------------------------

TEST(Benchmark_Str, stringify_strings_only__dotimes_10_000_000)
{
  LispleTest::SnippetBenchmark bm(
    R"((dotimes [n 10000000] (str "hello" ", " "world" "! " "foo" " " "bar" " " "baz")))");

  bm.run();
}

TEST(Benchmark_Str, stringify_strings_and_primitives__dotimes_10_000_000)
{
  LispleTest::SnippetBenchmark bm(
    R"((dotimes [n 10000000] (str "n=" n " doubled=" (* n 2) " even=" (= (mod n 2) 0))))");

  bm.run();
}

TEST(Benchmark_Str, stringify_strings_and_complex_objects__dotimes_10_000_000)
{
  // Pre-build the collections once; the benchmark measures only (str ...) overhead.
  LispleTest::SnippetBenchmark bm(
    R"((let [v [1 2 3 4 5]
             m {:name "Alice" :age 30 :active true :scores [10 20 30]}]
         (dotimes [n 10000000]
           (str "v=" v " m=" m " n=" n))))");

  bm.run();
}

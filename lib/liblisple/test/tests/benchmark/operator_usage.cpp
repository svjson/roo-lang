
#include "benchmark.h"
#include <gtest/gtest.h>

TEST(Benchmark_Operators, dotimes_mixed_basic_arithmetic_operators_100_000)
{
  LispleTest::SnippetBenchmark bm("dotimes_100_000_mixed_basic_arithmetic",
                                  "(dotimes [n 100000] (- (/ (* 1000 (+ n 1)) 2 4) 8))");

  bm.run();
}

TEST(Benchmark_Operators, dotimes_mixed_basic_arithmetic_operators_1_000_000)
{
  LispleTest::SnippetBenchmark bm("dotimes_1_000_000_mixed_basic_arithmetic",
                                  "(dotimes [n 1000000] (- (/ (* 1000 (+ n 1)) 2 4) 8))");

  bm.run();
}

TEST(Benchmark_Operators, dotimes_comparison_and_mixed_basic_arithmetic_operators_100_000)
{
  LispleTest::SnippetBenchmark bm(
    "dotimes_100_000_comparison_and_mixed_basic_arithmetic",
    "(dotimes [n 100000] (= (* n 2) (- (/ (* 1000 (+ n 1)) 2 4) 8)))");

  bm.run();
}

TEST(Benchmark_Operators, map_mixed_basic_arithmetic_operators_100_000)
{
  LispleTest::SnippetBenchmark bm(
    "map_100_000_mixed_basic_arithmetic",
    "(map (range 0 100000) (fn [n] (- (/ (* 1000 (+ n 1)) 2 4) 8)))");

  bm.run();
}

TEST(Benchmark_Operators, map_mixed_basic_arithmetic_operators_1_000_000)
{
  LispleTest::SnippetBenchmark bm(
    "map_1_000_000_mixed_basic_arithmetic",
    "(map (range 0 1000000) (fn [n] (- (/ (* 1000 (+ n 1)) 2 4) 8)))");

  bm.run();
}

TEST(Benchmark_Operators, filter_comparison_and_mixed_basic_arithmetic_operators_1_000_000)
{
  LispleTest::SnippetBenchmark bm(
    "filter_1_000_000_mixed_basic_arithmetic",
    "(filter (range 0 1000000) (fn [n] (= (- (/ (* 1000 (+ n 1)) 2 4) 8) 0)))");

  bm.run();
}

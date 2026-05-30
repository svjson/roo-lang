
#include "benchmark.h"
#include <gtest/gtest.h>
#include <roo/runtime.h>

TEST(Benchmark_DoTimes, benchmark_dotimes_100_000_with_addition)
{
  RooTest::SnippetBenchmark bm("dotimes_100_000_with_addition",
                               "(dotimes [n 100000] (+ n 1))");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_DoTimes, benchmark_dotimes_1_000_000_with_addition)
{
  RooTest::SnippetBenchmark bm("dotimes_1_000_000_with_addition",
                               "(dotimes [n 1000000] (+ n 1))");

  bm.run();
}

TEST(Benchmark_DoTimes, benchmark_dotimes_100_000_with_map_construction)
{
  RooTest::SnippetBenchmark bm("dotimes_100_000_with_map_construction",
                               "(dotimes [n 100000] {:value n :str-value (str n)})");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_DoTimes, benchmark_dotimes_1_000_000_with_map_construction)
{
  RooTest::SnippetBenchmark bm("dotimes_1_000_000_with_map_construction",
                               "(dotimes [n 1000000] {:value n :str-value (str n)})");

  bm.run();
}

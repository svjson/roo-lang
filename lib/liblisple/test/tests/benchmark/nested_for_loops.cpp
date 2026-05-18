
#include "benchmark/benchmark.h"
#include <gtest/gtest.h>

TEST(Benchmark_NestedFor, three_dimensional_loop_with_map_construction_100_x_100_x_100)
{
  LispleTest::SnippetBenchmark bm("nested_for_100_x_100_x_100_map_construction",
                                  R"(
    (let [height (range 0 100)
          width (range 0 100)
          depth (range 0 100)]
      (for [y height]
        (for [x width]
          (for [z depth]
            {:x x :y y :z z}))))
                                   )");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_NestedFor, three_dimensional_loop_with_map_construction_100_x_10000_x_10)
{
  LispleTest::SnippetBenchmark bm("nested_for_100_x_10000_x_10_map_construction",
                                  R"(
    (let [height (range 0 100)
          width (range 0 10000)
          depth (range 0 10)]
      (for [y height]
        (for [x width]
          (for [z depth]
            {:x x :y y :z z}))))
                                   )");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_NestedFor,
     six_dimensional_loop_with_map_construction_10_x_10_x_10_x_10_x_10_x_10)
{
  LispleTest::SnippetBenchmark bm("nested_for_10_x_10_x_10_x_10_x_10_x_10_map_construction",
                                  R"(
    (let [height (range 0 10)
          width (range 0 10)
          depth (range 0 10)]
      (for [y1 height]
        (for [x1 width]
          (for [z1 depth]
            (for [y2 height]
              (for [x2 width]
                (for [z2 depth]
                   {:x1 x1 :y1 y1 :z1 z1
                    :x2 x2 :y2 y2 :z2 z2})))))))
                                   )");

  bm.with_lower_iterations(1000).run();
}

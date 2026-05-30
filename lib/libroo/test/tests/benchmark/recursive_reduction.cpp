
#include <string>

const std::string NS__BM__NESTED_STATS = "(ns benchmark.nested-stats)";
#include "benchmark.h"
#include <gtest/gtest.h>

const std::string DEFUN_NESTED_STATS = R"(

(defun nested-stats [groups]
  (for [g groups]
    (let [vals (map g :value)]
      {:sum (apply + vals)
       :min (apply min vals)
       :max (apply max vals)})))
                                        )";

const std::string DEFUN_GENERATE_GROUPS = R"(
(defun generate-groups [num-groups group-size]
  (let [seed 123456789]
    (for [i (range 0 num-groups)]
      (for [j (range 0 group-size)]
        (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
        {:id j
         :value (- (mod seed 1000) 500)}))))
                                           )";
const std::string DEFUN_GENERATE_GROUPS_VAR = R"(
(defun generate-groups-var [num-groups max-group-size]
  (let [seed 123456789]
    (for [i (range 0 num-groups)]
      (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
      (let [size (+ 5 (mod seed max-group-size))]
        (for [j (range 0 size)]
          (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
          {:id j
           :value (- (mod seed 1000) 500)})))))
                                               )";

const std::string DEFUN_BENCHMARK_NESTED_STATS = R"(
(defun benchmark-nested-stats [num-groups group-size iterations]
  (let [groups (generate-groups num-groups group-size)
        result nil]

    (dotimes [i iterations]
      (set! [result] (nested-stats groups)))

    (count result)))
                                                  )";

const std::string DEFUN_BENCHMARK_NESTED_STATS_VAR_GROUPS = R"(
(defun benchmark-nested-stats-var [num-groups group-size iterations]
  (let [groups (generate-groups-var num-groups group-size)
        result nil]

    (dotimes [i iterations]
      (set! [result] (nested-stats groups)))

    (count result)))
                                                  )";

const std::string BENCHMARK__NESTED_STATS = NS__BM__NESTED_STATS + DEFUN_NESTED_STATS +
                                            DEFUN_GENERATE_GROUPS +
                                            DEFUN_BENCHMARK_NESTED_STATS;

const std::string BENCHMARK__NESTED_STATS_VAR = NS__BM__NESTED_STATS + DEFUN_NESTED_STATS +
                                                DEFUN_GENERATE_GROUPS_VAR +
                                                DEFUN_BENCHMARK_NESTED_STATS_VAR_GROUPS;

TEST(Benchmark_Workload, nested_stats__500_events__20_group_size__50_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__NESTED_STATS},
                               "benchmark.nested-stats",
                               "(benchmark-nested-stats 500 20 50)");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, nested_stats__1000_events__30_group_size__30_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__NESTED_STATS},
                               "benchmark.nested-stats",
                               "(benchmark-nested-stats 1000 30 30)");

  bm.run();
}

TEST(Benchmark_Workload, nested_stats__2000_events__40_group_size__20_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__NESTED_STATS},
                               "benchmark.nested-stats",
                               "(benchmark-nested-stats 2000 40 20)");

  bm.run();
}

TEST(Benchmark_Workload, nested_stats_var_groups__500_events__20_group_size__50_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__NESTED_STATS_VAR},
                               "benchmark.nested-stats",
                               "(benchmark-nested-stats-var 500 20 50)");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, nested_stats_var_groups__1000_events__30_group_size__30_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__NESTED_STATS_VAR},
                               "benchmark.nested-stats",
                               "(benchmark-nested-stats-var 1000 30 30)");

  bm.run();
}

TEST(Benchmark_Workload, nested_stats_var_groups__2000_events__40_group_size__20_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__NESTED_STATS_VAR},
                               "benchmark.nested-stats",
                               "(benchmark-nested-stats-var 2000 40 20)");

  bm.run();
}

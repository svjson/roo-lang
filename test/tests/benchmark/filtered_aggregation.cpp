
#include "benchmark.h"
#include <gtest/gtest.h>

const std::string DEFUN_PROCESS_ENTITIES = R"(
(defun process-entities [entities]
  (let [result {:count 0 :sum 0 :valid 0}]
    (do
      (for [e entities]
        (when (and e (:active e))
          (if-let [val (:value e)]
            (do
              (when (> val 10)
                (set! [:sum result] (+ (:sum result) val))
                (set! [:valid result] (+ 1 (:valid result))))
              (set! [:count result] (+ 1 (:count result)))
            (when-let [fallback (:fallback e)]
              (when (> fallback 5)
                (set! [:sum result] (+ (:sum result) fallback))))))))
      result)))
                                            )";
const std::string DEFUN_MAKE_ENTITIES = R"(
(defun make-entities [n]
  (for [i (range 1 n)]
    {:id i
     :active (even? i)
     :value (when (= 0 (mod i 3)) (* i 2))
     :fallback (when (= 0 (mod i 5)) (+ i 7))}))
                                         )";

const std::string DEFUN_MAKE_ENTITIES__ZEROP = R"(
(defun make-entities [n]
  (for [i (range 1 n)]
    {:id i
     :active (even? i)
     :value (when (zero? (mod i 3)) (* i 2))
     :fallback (when (zero? (mod i 5)) (+ i 7))}))
                                         )";

TEST(Workload, filtered_aggregation_10000_entities)
{
  LispleTest::SnippetBenchmark bm(
    "filtered_aggregation__10000_entities",
    {"(ns process)" + DEFUN_MAKE_ENTITIES + DEFUN_PROCESS_ENTITIES},
    "process",
    "(process-entities (make-entities 10000))");

  bm.run();
}

TEST(Benchmark_Workload, benchmark_filtered_aggregation_100_iterations_10000_entities)
{
  LispleTest::SnippetBenchmark bm(
    "filtered_aggregation__100_iterations_10000_entities",
    {"(ns process)" + DEFUN_MAKE_ENTITIES + DEFUN_PROCESS_ENTITIES},
    "process",
    "(dotimes [n 100] (process-entities (make-entities 10000)))");

  bm.run();
}

TEST(Benchmark_Workload, benchmark_filtered_aggregation_100_iterations_100000_entities)
{
  LispleTest::SnippetBenchmark bm(
    "filtered_aggregation__100_iterations_100000_entities",
    {"(ns process)" + DEFUN_MAKE_ENTITIES + DEFUN_PROCESS_ENTITIES},
    "process",
    "(dotimes [n 100] (process-entities (make-entities 100000)))");

  bm.run();
}

TEST(Benchmark_Workload,
     benchmark_filtered_aggregation_100_iterations_10000_entities__with_zerop)
{
  LispleTest::SnippetBenchmark bm(
    "filtered_aggregation__100_iterations_10000_entities__with_zerop",
    {"(ns process)" + DEFUN_MAKE_ENTITIES__ZEROP + DEFUN_PROCESS_ENTITIES},
    "process",
    "(dotimes [n 100] (process-entities (make-entities 10000)))");

  bm.run();
}

TEST(Benchmark_Workload,
     benchmark_filtered_aggregation_100_iterations_100000_entities__with_zerop)
{
  LispleTest::SnippetBenchmark bm(
    "filtered_aggregation__100_iterations_100000_entities__with_zerop",
    {"(ns process)" + DEFUN_MAKE_ENTITIES__ZEROP + DEFUN_PROCESS_ENTITIES},
    "process",
    "(dotimes [n 100] (process-entities (make-entities 100000)))");

  bm.run();
}

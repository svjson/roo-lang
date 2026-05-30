
#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

const std::string NS__BM__PROCESS = "(ns benchmark.process)";

const std::string DEFUN_PROCESS_EVENTS = R"(
(defun process-events [events threshold]
  (let [filtered
        (filter events (fn [e]
                         (and e (:active e))))

        kept
        (keep filtered (fn [e]
                         (when (> (:value e) threshold)
                           {:id (:id e) :value (:value e)})))

        cleaned
        (remove kept (fn [e]
                       (= (:value e) 0)))]

    (for [e cleaned]
      (case (:type e)
        :a (+ (:value e) 10)
        :b (* (:value e) 2)
        :c (- (:value e) 5)
        :default 0))))
                                          )";

const std::string DEFUN_GENERATE_EVENTS = R"(
     (defun generate-events [n]
       (let [seed 123456789]
         (for [i (range 0 n)]
           (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
           (let [v (mod seed 200)]
             (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
             (let [t (mod seed 3)]
               {:id i
                :active (not (= (mod v 5) 0))     ;; ~20% inactive
                :value (- v 100)                  ;; range -100..100
                :type (case t
                        0 :a
                        1 :b
                        :default :c)})))))
                                           )";

const std::string DEFUN_BENCHMARK_PROCESS = R"(
     (defun benchmark-process [n iterations threshold]
       (let [events (generate-events n)
             result nil]

         (dotimes [i iterations]
           (set! [result] (process-events events threshold)))

         ;; return something to prevent "optimization"
         (count result)))
                                            )";

const std::string BENCHMARK__PROCESS =
  NS__BM__PROCESS + DEFUN_PROCESS_EVENTS + DEFUN_GENERATE_EVENTS + DEFUN_BENCHMARK_PROCESS;

TEST(Benchmark_Workload, process_events__1000_events__50_iterations__20_threshold)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__PROCESS},
                               "benchmark.process",
                               "(benchmark-process 1000 50 20)");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, process_events__5000_events__80_iterations__20_threshold)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__PROCESS},
                               "benchmark.process",
                               "(benchmark-process 5000 50 20)");

  bm.run();
}

TEST(Benchmark_Workload, process_events__20000_events__20_iterations__20_threshold)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__PROCESS},
                               "benchmark.process",
                               "(benchmark-process 20000 20 20)");

  bm.run();
}

TEST(Benchmark_Workload, process_events__20000_events__50_iterations__20_threshold)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__PROCESS},
                               "benchmark.process",
                               "(benchmark-process 20000 50 20)");

  bm.run();
}

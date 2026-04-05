
#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

const std::string NS__BM__QUEUE = "(ns benchmark.queue)";

const std::string DEFUN_PROCESS_QUEUE = R"(
    (defun process-queue! [tasks]
      (let [i 0]
        (while (< i (count tasks))
          (let [task (nth tasks i)]
            (if (:done task)
              (do
                (remove-nth! tasks i)
                (set! [i] (- i 1)))
              (do
                (assoc! task :progress (+ (:progress task) 1))
                (when (> (:progress task) 10)
                  (assoc! task :done true)))))
          (set! [i] (+ i 1)))
        tasks))
                                         )";

const std::string DEFUN_GENERATE_TASKS = R"(
(defun generate-tasks [n]
  (let [seed 123456789]
    (for [i (range 0 n)]
      (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
      (let [p (mod seed 15)]   ;; progress 0–14
        {:id i
         :progress p
         :done (> p 10)}))))   ;; some already done
                                          )";

const std::string DEFUN_BENCHMARK_QUEUE = R"(
    (defun benchmark-queue [n iterations]
      (let [result nil]
        (dotimes [i iterations]
          (let [tasks (generate-tasks n)]
            (set! [result] (process-queue! tasks))))
        (count result)))
                                           )";

const std::string BENCHMARK__QUEUE =
  NS__BM__QUEUE + DEFUN_PROCESS_QUEUE + DEFUN_GENERATE_TASKS + DEFUN_BENCHMARK_QUEUE;

TEST(Benchmark_Workload, process_task_queue__2000_tasks__50_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__QUEUE},
                                  "benchmark.queue",
                                  "(benchmark-queue 2000 50)");

  bm.run();
}

TEST(Benchmark_Workload, process_task_queue__10000_tasks__20_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__QUEUE},
                                  "benchmark.queue",
                                  "(benchmark-queue 10000 20)");

  bm.run();
}

TEST(Benchmark_Workload, process_task_queue__20000_tasks__20_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__QUEUE},
                                  "benchmark.queue",
                                  "(benchmark-queue 20000 20)");

  bm.run();
}


#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

const std::string NS__BM__SLIDING_WINDOW = "(ns benchmark.sliding-window)";

const std::string DEFUN_SLIDING_WINDOW = R"(
(defun sliding-window [values window-size]
  (let [result []]
    (let [remaining values]
      (while (> (count remaining) window-size)
        (let [window (take window-size remaining)
              mn (apply min window)
              mx (apply max window)]
          (append! result {:min mn :max mx}))
        (set! [remaining] (tail remaining))))
    result))
                              )";

const std::string DEFUN_GENERATE_VALUES = R"(
(defun generate-values [n]
  (let [seed 123456789]
    (for [i (range 0 n)]
      (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
      (- (mod seed 1000) 500))))   ;; range -500..500
                                           )";

const std::string DEFUN_BENCHMARK_SLIDING_WINDOW = R"(
(defun benchmark-sliding-window [n window-size iterations]
  (let [values (generate-values n)
        result nil]

    (dotimes [i iterations]
      (set! [result] (sliding-window values window-size)))

    (count result)))
                                                    )";

const std::string BENCHMARK__SLIDING_WINDOW = NS__BM__SLIDING_WINDOW + DEFUN_SLIDING_WINDOW +
                                              DEFUN_GENERATE_VALUES +
                                              DEFUN_BENCHMARK_SLIDING_WINDOW;

TEST(Benchmark_Workload, sliding_window__1000_values__20_window_size__20_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__SLIDING_WINDOW},
                                  "benchmark.sliding-window",
                                  "(benchmark-sliding-window 1000 50 20)");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, sliding_window__10000_values__10_window_size__20_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__SLIDING_WINDOW},
                                  "benchmark.sliding-window",
                                  "(benchmark-sliding-window 10000 25 20)");

  bm.run();
}

TEST(Benchmark_Workload, sliding_window__20000_values__10_window_size__10_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__SLIDING_WINDOW},
                                  "benchmark.sliding-window",
                                  "(benchmark-sliding-window 20000 30 10)");

  bm.run();
}


#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

const std::string NS__BM__IMMUTABLE_UPDATE_WORLD = "(ns benchmark.immutable-update-world)";

const std::string DEFUN_UPDATE_WORLD = R"(
(defun update-world [entities]
  (let [result []]
    (for [e entities]
      (let [e2 (assoc e :x (+ (:x e) (or (:vx e) 0)))
            e3 (assoc e2 :y (+ (:y e2) (or (:vy e2) 0)))
            e4 (if (> (:x e3) 1000)
                 (dissoc e3 :vx)
                 e3)]
        (append! result e4)))
    result))
)";

const std::string DEFUN_GENERATE_ENTITIES = R"(
(defun generate-entities [n]
  (let [seed 123456789]
    (for [i (range 0 n)]
      (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
      (let [x (mod seed 1200)]
        (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
        (let [y (mod seed 1200)]
          (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
          (let [vx (- (mod seed 20) 10)]
            (set! [seed] (mod (+ (* seed 1103515245) 12345) 2147483648))
            (let [vy (- (mod seed 20) 10)]
              {:id i
               :x x
               :y y
               :vx vx
               :vy vy})))))))
                                             )";

const std::string DEFUN_BENCHMARK_UPDATE_WORLD = R"(
(defun benchmark-update-world [n iterations]
  (let [entities (generate-entities n)
        result nil]

    (dotimes [i iterations]
      (set! [result] (update-world entities)))

    (count result)))
                                                  )";

const std::string BENCHMARK__IMMUTABLE_UPDATE_WORLD =
  NS__BM__IMMUTABLE_UPDATE_WORLD + DEFUN_UPDATE_WORLD + DEFUN_GENERATE_ENTITIES +
  DEFUN_BENCHMARK_UPDATE_WORLD;

TEST(Benchmark_Workload, immutable_update_world__5000_entities__100_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__IMMUTABLE_UPDATE_WORLD},
                               "benchmark.immutable-update-world",
                               "(benchmark-update-world 5000 100)");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, immutable_update_world__10000_entities__100_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__IMMUTABLE_UPDATE_WORLD},
                               "benchmark.immutable-update-world",
                               "(benchmark-update-world 10000 100)");

  bm.run();
}

TEST(Benchmark_Workload, immutable_update_world__20000_entities__200_iterations)
{
  RooTest::SnippetBenchmark bm({BENCHMARK__IMMUTABLE_UPDATE_WORLD},
                               "benchmark.immutable-update-world",
                               "(benchmark-update-world 20000 200)");

  bm.run();
}

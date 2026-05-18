
#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

const std::string NS__BM__MUTABLE_UPDATE_WORLD = "(ns benchmark.mutable-update-world)";

const std::string DEFUN_UPDATE_WORLD_BANG = R"(
(defun update-world! [entities]
  (for [e entities]
    (do
      (when (and (:x e) (:vx e))
        (assoc! e :x (+ (:x e) (:vx e))))
      (when (and (:y e) (:vy e))
        (assoc! e :y (+ (:y e) (:vy e))))
      (when (and (:x e) (> (:x e) 1000))
        (dissoc! e :vx))))
  entities)
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

const std::string DEFUN_BENCHMARK_UPDATE_WORLD_BANG = R"(
(defun benchmark-update-world-bang [n iterations]
  (let [entities (generate-entities n)]
    (dotimes [i iterations]
      (update-world! entities))
    (count entities)))
                                                       )";

const std::string BENCHMARK__MUTABLE_UPDATE_WORLD =
  NS__BM__MUTABLE_UPDATE_WORLD + DEFUN_UPDATE_WORLD_BANG + DEFUN_GENERATE_ENTITIES +
  DEFUN_BENCHMARK_UPDATE_WORLD_BANG;

TEST(Benchmark_Workload, mutable_update_world__5000_entities__100_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__MUTABLE_UPDATE_WORLD},
                                  "benchmark.mutable-update-world",
                                  "(benchmark-update-world-bang 5000 100)");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, mutable_update_world__10000_entities__100_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__MUTABLE_UPDATE_WORLD},
                                  "benchmark.mutable-update-world",
                                  "(benchmark-update-world-bang 10000 100)");

  bm.run();
}

TEST(Benchmark_Workload, mutable_update_world__20000_entities__200_iterations)
{
  LispleTest::SnippetBenchmark bm({BENCHMARK__MUTABLE_UPDATE_WORLD},
                                  "benchmark.mutable-update-world",
                                  "(benchmark-update-world-bang 20000 200)");

  bm.run();
}

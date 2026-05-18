
#include <lisple/runtime.h>

#include "benchmark.h"
#include "fixtures/pixils_point.h"
#include "fixtures/point_vec.h"
#include "host/test_adapters/vectorgfx_native_adapters.h"
#include "host/test_adapters/vectorgfx_native_cachedprops_adapters.h"
#include <gtest/gtest.h>

const std::string NS__POINT_SORT = R"(
  (ns test.point-sort
    (:require [pixils.lisple.point :as pt]
              [pixils.point :as npt]))
)";

const std::string DEF_ITERATIONS = "(def ITERATIONS 1)";

const std::string DEFUN__SORT_POINTS = R"(
  (defun sort-points [points origin]
    (sort points
      (fn [a b]
        (< (pt/distance origin a)
           (pt/distance origin b)))))
                                        )";

TEST(Benchmark_Workload, point_sort__map_points)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__POINT_SORT + LispleTest::DEF_POINT_VEC_750,
     DEF_ITERATIONS,
     DEFUN__SORT_POINTS},
    "test.point-sort",
    "(dotimes [ITERATIONS] (sort-points point-vec {:x 0 :y 0}))");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, point_sort__native_points)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__POINT_SORT + LispleTest::DEF_POINT_VEC_750,
     LispleTest::DEF_NPOINT_VEC,
     DEF_ITERATIONS,
     DEFUN__SORT_POINTS},
    "test.point-sort",
    "(dotimes [ITERATIONS] (sort-points npoint-vec (npt/make-point {:x 0 :y 0})))");

  bm.with_lower_iterations(1000).run();
}

TEST(Benchmark_Workload, point_sort__native_propcache_points)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::CachedNative::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__POINT_SORT + LispleTest::DEF_POINT_VEC_750,
     LispleTest::DEF_NPOINT_VEC,
     DEF_ITERATIONS,
     DEFUN__SORT_POINTS},
    "test.point-sort",
    "(dotimes [ITERATIONS] (sort-points npoint-vec (npt/make-point {:x 0 :y 0})))");

  bm.with_lower_iterations(1000).run();
}

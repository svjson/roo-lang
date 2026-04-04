
#include <lisple/runtime.h>

#include "benchmark.h"
#include "fixtures/pixils_point.h"
#include "fixtures/point_vec.h"
#include "host/test_adapters/vectorgfx_native_adapters.h"
#include "host/test_adapters/vectorgfx_native_cachedprops_adapters.h"
#include <gtest/gtest.h>

const std::string NS__CENTROID = R"(
  (ns test.centroid
    (:require [pixils.lisple.point :as pt]
              [pixils.point :as npt]))
)";

const std::string DEF__THRESHOLD__200 = "(def THRESHOLD 200)";

const std::string DEF__PT_DISTANCE = "(def distance pt/distance)";
const std::string DEF__NPT_DISTANCE = "(def distance npt/distance)";

const std::string DEFUN__CALC_CENTROID = R"(
  (defun calculate-centroid [points]
    (for [p points]
      (let [sumx 0 sumy 0 count 0]
        (for [q points]
          (when (< (distance p q) THRESHOLD)
            (set! [sumx] (+ sumx (:x q)))
            (set! [sumy] (+ sumy (:y q)))
            (set! [count] (+ count 1))))
          ;; Centroid update omitted
           )))
                                          )";

TEST(Benchmark_Workload, point_centroid__pure_lisp)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__CENTROID + LispleTest::DEF_POINT_VEC_500 + DEF__THRESHOLD__200,
     DEF__PT_DISTANCE,
     DEFUN__CALC_CENTROID},
    "test.centroid",
    R"(
(calculate-centroid point-vec)
     )");

  bm.run();
}

TEST(Benchmark_Workload, point_centroid__map_points__native_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__CENTROID + LispleTest::DEF_POINT_VEC_500 + DEF__THRESHOLD__200,
     DEF__NPT_DISTANCE,
     DEFUN__CALC_CENTROID},
    "test.centroid",
    R"(
(calculate-centroid point-vec)
     )");

  bm.run();
}

TEST(Benchmark_Workload, point_centroid__native_propcache_points__lisp_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::CachedNative::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__CENTROID + LispleTest::DEF_POINT_VEC_500 + LispleTest::DEF_NPOINT_VEC,
     DEF__THRESHOLD__200,
     DEF__PT_DISTANCE,
     DEFUN__CALC_CENTROID},
    "test.centroid",
    R"(
(calculate-centroid npoint-vec)
     )");

  bm.run();
}

TEST(Benchmark_Workload, point_centroid__native_points__lisp_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__CENTROID + LispleTest::DEF_POINT_VEC_500 + LispleTest::DEF_NPOINT_VEC,
     DEF__THRESHOLD__200,
     DEF__PT_DISTANCE,
     DEFUN__CALC_CENTROID},
    "test.centroid",
    R"(
(calculate-centroid npoint-vec)
     )");

  bm.run();
}

TEST(Benchmark_Workload, point_centroid__native_points__native_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__CENTROID + LispleTest::DEF_POINT_VEC_500 + LispleTest::DEF_NPOINT_VEC,
     DEF__THRESHOLD__200,
     DEF__NPT_DISTANCE,
     DEFUN__CALC_CENTROID},
    "test.centroid",
    R"(
(calculate-centroid npoint-vec)
     )");

  bm.run();
}

TEST(Benchmark_Workload, point_centroid__native_propcache_points__native_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::CachedNative::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__CENTROID + LispleTest::DEF_POINT_VEC_500 + LispleTest::DEF_NPOINT_VEC,
     DEF__THRESHOLD__200,
     DEF__NPT_DISTANCE,
     DEFUN__CALC_CENTROID},
    "test.centroid",
    R"(
(calculate-centroid npoint-vec)
     )");

  bm.run();
}



#include "runtime_fixture.h"

#include "benchmark.h"
#include "fixtures/pixils_point.h"
#include "fixtures/point_vec.h"
#include "host/test_adapters/vectorgfx_native_adapters.h"
#include "host/test_adapters/vectorgfx_native_cachedprops_adapters.h"
#include <gtest/gtest.h>


using DISABLED_GeneratePoints = LispleTest::RuntimeTestFixture;
using PointDistanceMatrixBenchmark = LispleTest::RuntimeTestFixture;
const std::string NS__MATRIX = R"(
  (ns test.matrix
    (:require [pixils.lisple.point :as pt]
              [pixils.point :as npt]))
)";

/**
 * Generate the point example data and flush to stdout
 *
 * Kept for potential re-generation purposes
 */
TEST_F(DISABLED_GeneratePoints, gen_points)
{
  runtime.eval(LispleTest::DEFUN_GENERATE_POINTS);
  auto points = runtime.eval("(generate-points 750)");
  std::cout << points->to_string() << std::endl;
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__pure_lisp)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__MATRIX + LispleTest::DEF_POINT_VEC_750},
    "test.matrix",
    R"(
(for [i point-vec]
  (for [j point-vec]
    (pt/distance i j)))
                                   )");

  bm.run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__map_points__native_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__MATRIX + LispleTest::DEF_POINT_VEC_750},
    "test.matrix",
    R"(
(for [i point-vec]
  (for [j point-vec]
    (npt/distance i j)))
                                   )");

  bm.run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__native_points__lisple_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::CachedNative::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__MATRIX + LispleTest::DEF_POINT_VEC_750 + LispleTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (pt/distance i j)))
                                   )");

  bm.run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__native_propcache_points__lisple_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__MATRIX + LispleTest::DEF_POINT_VEC_750 + LispleTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (pt/distance i j)))
                                   )");

  bm.run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__native_points__native_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::Native::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__MATRIX + LispleTest::DEF_POINT_VEC_750 + LispleTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (npt/distance i j)))
                                   )");

  bm.run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__native_propcache_points__native_distance_fn)
{
  LispleTest::SnippetBenchmark bm(
    {{"pixils.point", LispleTest::CachedNative::PointNamespace("pixils.point")}},
    {LispleTest::pixils_point_ns__as("pixils.lisple.point"),
     NS__MATRIX + LispleTest::DEF_POINT_VEC_750 + LispleTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (npt/distance i j)))
                                   )");

  bm.run();
}

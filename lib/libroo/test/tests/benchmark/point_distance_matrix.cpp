

#include "benchmark.h"
#include "fixtures/pixils_point.h"
#include "fixtures/point_vec.h"
#include "host/test_adapters/vectorgfx_native_adapters.h"
#include "host/test_adapters/vectorgfx_native_cachedprops_adapters.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DISABLED_GeneratePoints = RooTest::RuntimeTestFixture;
using PointDistanceMatrixBenchmark = RooTest::RuntimeTestFixture;
const std::string NS__MATRIX = R"(
  (ns test.matrix
    (:require [pixils.roo.point :as pt]
              [pixils.point :as npt]))
)";

/**
 * Generate the point example data and flush to stdout
 *
 * Kept for potential re-generation purposes
 */
TEST_F(DISABLED_GeneratePoints, gen_points)
{
  runtime.eval(RooTest::DEFUN_GENERATE_POINTS);
  auto points = runtime.eval("(generate-points 750)");
  std::cout << points->to_string() << std::endl;
}

TEST(Benchmark_Lowering, point_vec_500_setup__lower_x1000__no_exec)
{
  RooTest::SnippetBenchmark bm("point_vec_500_setup__lower_x1000__no_exec",
                               RooTest::DEF_POINT_VEC_500);

  bm.with_lower_iterations(1000).without_execution().run();
}

TEST(Benchmark_Lowering, point_vec_750_setup__lower_x1000__no_exec)
{
  RooTest::SnippetBenchmark bm("point_vec_750_setup__lower_x1000__no_exec",
                               RooTest::DEF_POINT_VEC_750);

  bm.with_lower_iterations(1000).without_execution().run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__pure_lisp)
{
  RooTest::SnippetBenchmark bm(
    {{"pixils.point", RooTest::Native::PointNamespace("pixils.point")}},
    {RooTest::pixils_point_ns__as("pixils.roo.point"),
     NS__MATRIX + RooTest::DEF_POINT_VEC_750},
    "test.matrix",
    R"(
(for [i point-vec]
  (for [j point-vec]
    (pt/distance i j)))
                                   )");

  bm.with_lower_iterations(1000).run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__map_points__native_distance_fn)
{
  RooTest::SnippetBenchmark bm(
    {{"pixils.point", RooTest::Native::PointNamespace("pixils.point")}},
    {RooTest::pixils_point_ns__as("pixils.roo.point"),
     NS__MATRIX + RooTest::DEF_POINT_VEC_750},
    "test.matrix",
    R"(
(for [i point-vec]
  (for [j point-vec]
    (npt/distance i j)))
                                   )");

  bm.with_lower_iterations(1000).run();
}

TEST_F(PointDistanceMatrixBenchmark, point_distance_matrix__native_points__roo_distance_fn)
{
  RooTest::SnippetBenchmark bm(
    {{"pixils.point", RooTest::CachedNative::PointNamespace("pixils.point")}},
    {RooTest::pixils_point_ns__as("pixils.roo.point"),
     NS__MATRIX + RooTest::DEF_POINT_VEC_750 + RooTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (pt/distance i j)))
                                   )");

  bm.with_lower_iterations(1000).run();
}

TEST_F(PointDistanceMatrixBenchmark,
       point_distance_matrix__native_propcache_points__roo_distance_fn)
{
  RooTest::SnippetBenchmark bm(
    {{"pixils.point", RooTest::Native::PointNamespace("pixils.point")}},
    {RooTest::pixils_point_ns__as("pixils.roo.point"),
     NS__MATRIX + RooTest::DEF_POINT_VEC_750 + RooTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (pt/distance i j)))
                                   )");

  bm.with_lower_iterations(1000).run();
}

TEST_F(PointDistanceMatrixBenchmark,
       point_distance_matrix__native_points__native_distance_fn)
{
  RooTest::SnippetBenchmark bm(
    {{"pixils.point", RooTest::Native::PointNamespace("pixils.point")}},
    {RooTest::pixils_point_ns__as("pixils.roo.point"),
     NS__MATRIX + RooTest::DEF_POINT_VEC_750 + RooTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (npt/distance i j)))
                                   )");

  bm.with_lower_iterations(1000).run();
}

TEST_F(PointDistanceMatrixBenchmark,
       point_distance_matrix__native_propcache_points__native_distance_fn)
{
  RooTest::SnippetBenchmark bm(
    {{"pixils.point", RooTest::CachedNative::PointNamespace("pixils.point")}},
    {RooTest::pixils_point_ns__as("pixils.roo.point"),
     NS__MATRIX + RooTest::DEF_POINT_VEC_750 + RooTest::DEF_NPOINT_VEC},
    "test.matrix",
    R"(
(for [i npoint-vec]
  (for [j npoint-vec]
    (npt/distance i j)))
                                   )");

  bm.with_lower_iterations(1000).run();
}

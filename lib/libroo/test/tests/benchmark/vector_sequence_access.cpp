
#include <cstddef>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include <roo/adapter.h>
#include <roo/namespace.h>
#include <roo/runtime/value.h>

#include "benchmark.h"
#include <gtest/gtest.h>

namespace
{
  constexpr size_t NATIVE_VECTOR_SIZE = 64 * 1024;
  constexpr int ACCESS_ITERATIONS = 100;

  std::vector<int>& native_values()
  {
    static std::vector<int> values = []
    {
      std::vector<int> v(NATIVE_VECTOR_SIZE);
      std::iota(v.begin(), v.end(), 0);
      return v;
    }();

    return values;
  }

  Roo::sptr_val runtime_values()
  {
    Roo::sptr_val_v values;
    values.reserve(NATIVE_VECTOR_SIZE);
    for (size_t i = 0; i < NATIVE_VECTOR_SIZE; i++)
    {
      values.push_back(Roo::Value::number(static_cast<int>(i)));
    }

    return Roo::Value::vector(std::move(values));
  }

  std::map<const std::string, Roo::Namespace> native_sequence_namespaces()
  {
    Roo::Namespace ns("bench.native-seq");
    ns.store("native-values", Roo::NativeStdVectorAdapter<int>::make_ref(native_values()));
    ns.store("rt-values", runtime_values());
    ns.store("half-index", Roo::Value::number(static_cast<int>(NATIVE_VECTOR_SIZE / 2)));
    ns.store("iterations", Roo::Value::number(ACCESS_ITERATIONS));

    return {{"bench.native-seq", std::move(ns)}};
  }

  RooTest::SnippetBenchmark vector_seq_benchmark(const std::string& case_name,
                                                 const std::string& input)
  {
    return RooTest::SnippetBenchmark(case_name,
                                     native_sequence_namespaces(),
                                     {},
                                     "bench.native-seq",
                                     input);
  }
} // namespace

TEST(Benchmark_VectorSequenceAccess, native_vector_nth_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_nth_64k__100_iterations",
                                 "(dotimes [n iterations] (nth native-values half-index))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_head_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_head_64k__100_iterations",
                                 "(dotimes [n iterations] (head native-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_last_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_last_64k__100_iterations",
                                 "(dotimes [n iterations] (last native-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_take_1_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_take_1_64k__100_iterations",
                                 "(dotimes [n iterations] (take 1 native-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_contains_first_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_contains_first_64k__100_iterations",
                                 "(dotimes [n iterations] (contains? native-values 0))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_rand_nth_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_rand_nth_64k__100_iterations",
                                 "(dotimes [n iterations] (rand-nth native-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_tail_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_tail_64k__100_iterations",
                                 "(dotimes [n iterations] (tail native-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_partition_256_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_partition_256_64k__100_iterations",
                                 "(dotimes [n iterations] (partition 256 native-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_concat_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_concat_64k__100_iterations",
                                 "(dotimes [n iterations] (concat native-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, native_vector_contains_missing_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("native_vector_contains_missing_64k__100_iterations",
                                 "(dotimes [n iterations] (contains? native-values -1))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_nth_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_nth_64k__100_iterations",
                                 "(dotimes [n iterations] (nth rt-values half-index))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_head_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_head_64k__100_iterations",
                                 "(dotimes [n iterations] (head rt-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_last_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_last_64k__100_iterations",
                                 "(dotimes [n iterations] (last rt-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_take_1_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_take_1_64k__100_iterations",
                                 "(dotimes [n iterations] (take 1 rt-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_contains_first_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_contains_first_64k__100_iterations",
                                 "(dotimes [n iterations] (contains? rt-values 0))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_rand_nth_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_rand_nth_64k__100_iterations",
                                 "(dotimes [n iterations] (rand-nth rt-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_tail_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_tail_64k__100_iterations",
                                 "(dotimes [n iterations] (tail rt-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_partition_256_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_partition_256_64k__100_iterations",
                                 "(dotimes [n iterations] (partition 256 rt-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_concat_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_concat_64k__100_iterations",
                                 "(dotimes [n iterations] (concat rt-values))");

  bm.run();
}

TEST(Benchmark_VectorSequenceAccess, rt_vector_contains_missing_64k__100_iterations)
{
  auto bm = vector_seq_benchmark("rt_vector_contains_missing_64k__100_iterations",
                                 "(dotimes [n iterations] (contains? rt-values -1))");

  bm.run();
}

#include <string>

#include <lisple/runtime/exec_node.h>

#include "benchmark/benchmark.h"
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

int main(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "--benchmark")
    {
      LispleTest::logging_enabled = true;
    }
    else if (std::string(argv[i]) == "--skip-benchmarks")
    {
      LispleTest::skip_benchmark_tests = true;
    }
  }

  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();

  LispleTest::print_counters();

  return ret;
}

#include <string>

#include <roo/runtime/exec_node.h>

#include "benchmark/benchmark.h"
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

int main(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "--benchmark")
    {
      RooTest::logging_enabled = true;
    }
    else if (std::string(argv[i]) == "--skip-benchmarks")
    {
      RooTest::skip_benchmark_tests = true;
    }
  }

  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();

  RooTest::print_counters();

  return ret;
}

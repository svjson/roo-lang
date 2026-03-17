#include "benchmark/benchmark.h"
#include <chrono>
#include <cstdlib>
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
  }

  std::srand(std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
               .count());
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}

#include <string>

#include "benchmark.h"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "--benchmark")
    {
      RooTest::logging_enabled = true;
    }
  }

  ::testing::InitGoogleTest(&argc, argv);
  const int result = RUN_ALL_TESTS();
  RooTest::print_counters();
  return result;
}

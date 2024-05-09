#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

#include <cstdlib>
#include <chrono>

int main(int argc, char **argv)
{
  std::srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}

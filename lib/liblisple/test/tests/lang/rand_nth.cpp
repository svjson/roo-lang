
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using RandNth = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * RandNthFunction - (rand-nth [...])
 * ===================================
 */

TEST_F(RandNth, all_elements_possible)
{
  // Given
  std::vector freq{0, 0, 0, 0, 0};

  // When
  for (int i = 0; i < 5000; i++)
  {
    int num = runtime.eval("(rand-nth [0 1 2 3 4])")->i64();
    freq[num]++;
  }

  // Then
  for (int num_freq : freq)
  {
    EXPECT_NE(num_freq, 0);
  }
}

TEST_F(RandNth, single_element)
{
  // Given
  for (int i = 0; i < 1000; i++)
  {
    int num = runtime.eval("(rand-nth [8])")->i64();
    EXPECT_EQ(num, 8);
  }
}

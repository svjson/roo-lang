
#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RandNth = RooTest::RuntimeTestFixture;
/*
 * ===================================
 * RandNthFunction - (rand-nth [...])
 * ===================================
 */

TEST_F(RandNth, all_elements_possible)
{
  // Given
  runtime.eval("(random-seed! 1234)");
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

TEST_F(RandNth, string_single_element)
{
  EXPECT_EQ(runtime.eval(R"((rand-nth "a"))")->to_string(), "'a'");
}

TEST_F(RandNth, native_vector_adapter_single_element)
{
  std::vector<int> single = {2};
  runtime.get_current_namespace().store("single",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(single));

  EXPECT_EQ(runtime.eval("(rand-nth single)")->to_string(), "2");
}

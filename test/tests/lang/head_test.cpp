
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(HeadFunction, head_vector)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval("(head [8 9 10])")->to_string(), "8");
}

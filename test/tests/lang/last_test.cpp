
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(LastFunction, last_vector)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval("(last [8 9 10])")->to_string(), "10");
}

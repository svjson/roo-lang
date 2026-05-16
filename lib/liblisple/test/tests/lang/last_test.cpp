
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using LastFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * LastFunction - (last [...])
 * ===================================
 */

TEST_F(LastFunction, last_vector)
{
  // Given
  EXPECT_EQ(runtime.eval("(last [8 9 10])")->to_string(), "10");
}

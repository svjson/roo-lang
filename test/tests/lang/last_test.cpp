
#include <lisple/adapter.h>
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using LastFunction = LispleTest::RuntimeTestFixture;
/*
 * ===================================
 * LastFunction - (last [...])
 * ===================================
 */

TEST_F(LastFunction, last_stdvector_int)
{
  // Given
  std::vector<int> int_v{50, 100, 90};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(*runtime.eval("(last [1 2 3])"), *Lisple::RTValue::number(3));
  EXPECT_EQ(*runtime.eval("(last wrapped-vec)"), *Lisple::RTValue::number(90));
}

TEST_F(LastFunction, last_vector)
{
  // Given
  EXPECT_EQ(runtime.eval("(last [8 9 10])")->to_string(), "10");
}

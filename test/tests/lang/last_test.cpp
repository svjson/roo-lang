
#include <lisple/adapter.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ===================================
 * LastFunction - (last [...])
 * ===================================
 */

TEST(LastFunction, last_stdvector_int)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v{50, 100, 90};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(*runtime.eval("(last [1 2 3])"), *Lisple::RTValue::number(3));
  EXPECT_EQ(*runtime.eval("(last wrapped-vec)"), *Lisple::RTValue::number(90));
}

TEST(LastFunction, last_vector)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval("(last [8 9 10])")->to_string(), "10");
}

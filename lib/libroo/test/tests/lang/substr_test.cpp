#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SubstrFunction = RooTest::RuntimeTestFixture;
TEST_F(SubstrFunction, extracts_substrings)
{
  // Given
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2))"), *Roo::Value::string("cdef"));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2 3))"), *Roo::Value::string("cde"));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 0 2))"), *Roo::Value::string("ab"));
}

TEST_F(SubstrFunction, out_of_range_and_empty_lengths_yield_empty_string)
{
  // Given
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 20))"), *Roo::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" -1))"), *Roo::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2 0))"), *Roo::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2 -4))"), *Roo::Value::string(""));
}

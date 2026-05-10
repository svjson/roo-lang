#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(SubstrFunction, extracts_substrings)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2))"), *Lisple::RTValue::string("cdef"));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2 3))"), *Lisple::RTValue::string("cde"));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 0 2))"), *Lisple::RTValue::string("ab"));
}

TEST(SubstrFunction, out_of_range_and_empty_lengths_yield_empty_string)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 20))"), *Lisple::RTValue::string(""));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" -1))"), *Lisple::RTValue::string(""));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2 0))"), *Lisple::RTValue::string(""));
  EXPECT_EQ(*runtime.eval(R"((substr "abcdef" 2 -4))"), *Lisple::RTValue::string(""));
}

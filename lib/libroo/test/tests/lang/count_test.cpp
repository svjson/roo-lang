
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CountFunction = RooTest::RuntimeTestFixture;
/*
 * ===================================
 * CountFunction - (count [...])
 *               - (count "a string")
 * ===================================
 */

TEST_F(CountFunction, count_seq_length)
{
  // Given
  // Then
  EXPECT_EQ(*runtime.eval("(count [])"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(count ['a' 'b'])"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(count [1 2 3])"), *Roo::Value::number(3));
  EXPECT_EQ(*runtime.eval("(count [0 0 0 0 0])"), *Roo::Value::number(5));
}

TEST_F(CountFunction, count_str_length)
{
  // Given
  EXPECT_EQ(*runtime.eval("(count \"\")"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(count \" \")"), *Roo::Value::number(1));
  EXPECT_EQ(*runtime.eval("(count \"..\")"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(count \" !\")"), *Roo::Value::number(2));
  EXPECT_EQ(*runtime.eval("(count \"abc\")"), *Roo::Value::number(3));
  EXPECT_EQ(*runtime.eval("(count \"Where is my garmonbozia?\")"), *Roo::Value::number(24));
  EXPECT_EQ(*runtime.eval("(count \"0123456789\")"), *Roo::Value::number(10));
  EXPECT_EQ(*runtime.eval("(count {:a 1 :b 2})"), *Roo::Value::number(2));
}

TEST_F(CountFunction, nil_is_empty_and_other_scalars_remain_one)
{
  EXPECT_EQ(*runtime.eval("(count nil)"), *Roo::Value::number(0));
  EXPECT_EQ(*runtime.eval("(count :hey-joe)"), *Roo::Value::number(1));
}

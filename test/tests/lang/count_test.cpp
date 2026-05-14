
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CountFunction = LispleTest::RuntimeTestFixture;
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
  EXPECT_EQ(*runtime.eval("(count [])"), *Lisple::Value::number(0));
  EXPECT_EQ(*runtime.eval("(count ['a' 'b'])"), *Lisple::Value::number(2));
  EXPECT_EQ(*runtime.eval("(count [1 2 3])"), *Lisple::Value::number(3));
  EXPECT_EQ(*runtime.eval("(count [0 0 0 0 0])"), *Lisple::Value::number(5));
}

TEST_F(CountFunction, count_str_length)
{
  // Given
  EXPECT_EQ(*runtime.eval("(count \"\")"), *Lisple::Value::number(0));
  EXPECT_EQ(*runtime.eval("(count \" \")"), *Lisple::Value::number(1));
  EXPECT_EQ(*runtime.eval("(count \"..\")"), *Lisple::Value::number(2));
  EXPECT_EQ(*runtime.eval("(count \" !\")"), *Lisple::Value::number(2));
  EXPECT_EQ(*runtime.eval("(count \"abc\")"), *Lisple::Value::number(3));
  EXPECT_EQ(*runtime.eval("(count \"Where is my garmonbozia?\")"),
            *Lisple::Value::number(24));
  EXPECT_EQ(*runtime.eval("(count \"0123456789\")"), *Lisple::Value::number(10));
  EXPECT_EQ(*runtime.eval("(count {:a 1 :b 2})"), *Lisple::Value::number(2));
}

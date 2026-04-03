
#include <lisple/adapter.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ===================================
 * CountFunction - (count [...])
 *               - (count "a string")
 * ===================================
 */

TEST(CountFunction, count_seq_length)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v{50, 100, 90};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(*runtime.eval("(count [])"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*runtime.eval("(count ['a' 'b'])"), *Lisple::RTValue::number(2));
  EXPECT_EQ(*runtime.eval("(count [1 2 3])"), *Lisple::RTValue::number(3));
  EXPECT_EQ(*runtime.eval("(count [0 0 0 0 0])"), *Lisple::RTValue::number(5));
  EXPECT_EQ(*runtime.eval("(count wrapped-vec)"), *Lisple::RTValue::number(3));
}

TEST(CountFunction, count_str_length)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(count \"\")"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*runtime.eval("(count \" \")"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*runtime.eval("(count \"..\")"), *Lisple::RTValue::number(2));
  EXPECT_EQ(*runtime.eval("(count \" !\")"), *Lisple::RTValue::number(2));
  EXPECT_EQ(*runtime.eval("(count \"abc\")"), *Lisple::RTValue::number(3));
  EXPECT_EQ(*runtime.eval("(count \"Where is my garmonbozia?\")"),
            *Lisple::RTValue::number(24));
  EXPECT_EQ(*runtime.eval("(count \"0123456789\")"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(count {:a 1 :b 2})"), *Lisple::RTValue::number(2));
}

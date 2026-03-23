
#include <gtest/gtest.h>
#include <lisple/adapter.h>
#include <lisple/runtime.h>

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
  EXPECT_EQ(*runtime.eval("(count [])"), Lisple::Number(0));
  EXPECT_EQ(*runtime.eval("(count ['a' 'b'])"), Lisple::Number(2));
  EXPECT_EQ(*runtime.eval("(count [1 2 3])"), Lisple::Number(3));
  EXPECT_EQ(*runtime.eval("(count [0 0 0 0 0])"), Lisple::Number(5));
  EXPECT_EQ(*runtime.eval("(count wrapped-vec)"), Lisple::Number(3));
}

TEST(CountFunction, count_str_length)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(count \"\")"), Lisple::Number(0));
  EXPECT_EQ(*runtime.eval("(count \" \")"), Lisple::Number(1));
  EXPECT_EQ(*runtime.eval("(count \"..\")"), Lisple::Number(2));
  EXPECT_EQ(*runtime.eval("(count \" !\")"), Lisple::Number(2));
  EXPECT_EQ(*runtime.eval("(count \"abc\")"), Lisple::Number(3));
  EXPECT_EQ(*runtime.eval("(count \"Where is my garmonbozia?\")"), Lisple::Number(24));
  EXPECT_EQ(*runtime.eval("(count \"0123456789\")"), Lisple::Number(10));
  EXPECT_EQ(*runtime.eval("(count {:a 1 :b 2})"), Lisple::Number(2));
}

#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(StrFunction, concat_strings)
{
  // Given
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval(R"((str "a" "b" "c"))")->to_string(), "\"abc\"");
  EXPECT_EQ(runtime.eval(R"((str 'a' 'b' 'c'))")->to_string(), "\"abc\"");
  EXPECT_EQ(runtime.eval(R"((str "a" 1 :rust))")->to_string(), "\"a1:rust\"");
  EXPECT_EQ(runtime.eval(R"((str [1 2 3]))")->to_string(), "\"[1 2 3]\"");
  EXPECT_EQ(runtime.eval(R"((str 'a' false {:a 1}))")->to_string(), R"("afalse{:a 1}")");
  EXPECT_EQ(runtime.eval(R"((str :key " " nil))")->to_string(), R"(":key nil")");
}

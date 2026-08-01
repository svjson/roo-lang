
#include <roo/data/string_literal.h>

#include <gtest/gtest.h>

TEST(StringLiteral, escapes_empty_string)
{
  EXPECT_EQ(Roo::escaped_string_literal(""), R"("")");
}

TEST(StringLiteral, escapes_quotes_backslashes_and_common_control_characters)
{
  EXPECT_EQ(Roo::escaped_string_literal("quoted \"value\" with \\ slash\nand\ttab\r"),
            R"("quoted \"value\" with \\ slash\nand\ttab\r")");
}

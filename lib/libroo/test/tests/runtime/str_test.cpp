
#include <roo/runtime/str.h>

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

TEST(Utf8Scalars, splits_complete_scalars)
{
  EXPECT_EQ(Roo::utf8_scalars("ä猫🙂"), std::vector<std::string>({"a", "̈", "猫", "🙂"}));
}

TEST(Utf8Scalars, preserves_malformed_bytes_as_individual_elements)
{
  EXPECT_EQ(Roo::utf8_scalars("\xc3x"), std::vector<std::string>({"\xc3", "x"}));
}

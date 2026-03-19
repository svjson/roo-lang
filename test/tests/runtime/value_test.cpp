
#include <gtest/gtest.h>
#include <lisple/runtime/value.h>

TEST(RTValue_keyword, equals)
{
  auto kind = Lisple::RTValue::keyword("kind");
  auto kind_ = Lisple::RTValue::keyword("kind");
  auto value = Lisple::RTValue::keyword("value");

  EXPECT_TRUE(kind == kind);
  EXPECT_TRUE(kind == kind_);
  EXPECT_FALSE(kind == value);
  EXPECT_FALSE(value == kind);
}

TEST(RTValue_map_entry, get_existing_single_keyword_key)
{
  // Given
  const std::vector<Lisple::RTValue> map_values{Lisple::RTValue::keyword("amount"),
                                                Lisple::RTValue::number(40)};
  auto lookup_key = Lisple::RTValue::keyword("amount");

  auto [key, value] = Lisple::map_entry(map_values, lookup_key);

  EXPECT_NE(key, nullptr);
  EXPECT_NE(value, nullptr);
}


#include <gtest/gtest.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/value.h>

TEST(Dict_map_entry, get_existing_single_keyword_key)
{
  // Given
  const Lisple::sptr_rtval_v map_values{Lisple::RTValue::keyword("amount"),
                                        Lisple::RTValue::number(40)};
  auto lookup_key = Lisple::RTValue::keyword("amount");

  auto [key, value] = Lisple::Dict::map_entry(map_values, *lookup_key);

  EXPECT_NE(key, nullptr);
  EXPECT_NE(value, nullptr);
}

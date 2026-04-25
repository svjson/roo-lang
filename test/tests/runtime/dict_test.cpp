
#include <lisple/runtime.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

TEST(Dict_map_entry, get_existing_single_keyword_key)
{
  // Given
  const Lisple::sptr_rtval_v map_values{Lisple::RTValue::keyword("amount"),
                                        Lisple::RTValue::number(40)};
  auto lookup_key = Lisple::RTValue::keyword("amount");

  // When
  auto [key, value] = Lisple::Dict::map_entry(map_values, *lookup_key);

  // Then
  EXPECT_NE(key, nullptr);
  EXPECT_NE(value, nullptr);
}

TEST(Dict_keys, get_numeric_char_keys)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::sptr_rtval map = runtime.eval(R"(
  (def num-map (reduce "0123456789"
                {}
                (fn [glyphs num]
                  (let [index (int (str num))]
                    (merge glyphs
                           {num {:x (* index 13)
                                 :y 0
                                 :w 13
                                 :h 23}})))))
                )");

  // When
  Lisple::sptr_rtval_v keys = Lisple::Dict::keys(*map);

  // Then
  EXPECT_EQ(keys.size(), 10);
  EXPECT_EQ(Lisple::RTValue::vector(keys)->to_string(),
            "['0' '1' '2' '3' '4' '5' '6' '7' '8' '9']");
}

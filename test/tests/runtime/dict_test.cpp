
#include <lisple/runtime/dict.h>
#include <lisple/runtime/value.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Dict_map_entry = LispleTest::RuntimeTestFixture;
using Dict_keys = LispleTest::RuntimeTestFixture;
TEST_F(Dict_map_entry, get_existing_single_keyword_key)
{
  // Given
  const Lisple::sptr_val_v map_values{Lisple::Value::keyword("amount"),
                                      Lisple::Value::number(40)};
  auto lookup_key = Lisple::Value::keyword("amount");

  // When
  auto [key, value] = Lisple::Dict::map_entry(map_values, *lookup_key);

  // Then
  EXPECT_NE(key, nullptr);
  EXPECT_NE(value, nullptr);
}

TEST_F(Dict_keys, get_numeric_char_keys)
{
  // Given
  Lisple::sptr_val map = runtime.eval(R"(
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
  Lisple::sptr_val_v keys = Lisple::Dict::keys(*map);

  // Then
  EXPECT_EQ(keys.size(), 10);
  EXPECT_EQ(Lisple::Value::vector(keys)->to_string(),
            "['0' '1' '2' '3' '4' '5' '6' '7' '8' '9']");
}


#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Dict_map_entry = RooTest::RuntimeTestFixture;
using Dict_keys = RooTest::RuntimeTestFixture;
TEST_F(Dict_map_entry, get_existing_single_keyword_key)
{
  // Given
  const Roo::sptr_val_v map_values{Roo::Value::keyword("amount"), Roo::Value::number(40)};
  auto lookup_key = Roo::Value::keyword("amount");

  // When
  auto [key, value] = Roo::Dict::map_entry(map_values, *lookup_key);

  // Then
  EXPECT_NE(key, nullptr);
  EXPECT_NE(value, nullptr);
}

TEST_F(Dict_map_entry, dotted_keyword_can_be_used_as_map_key)
{
  auto result = runtime.eval("(:ui/button.primary {:ui/button.primary 42})");

  EXPECT_EQ(*result, *Roo::Value::number(42));
}

TEST_F(Dict_keys, get_numeric_char_keys)
{
  // Given
  Roo::sptr_val map = runtime.eval(R"(
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
  Roo::sptr_val_v keys = Roo::Dict::keys(*map);

  // Then
  EXPECT_EQ(keys.size(), 10);
  EXPECT_EQ(Roo::Value::vector(keys)->to_string(),
            "['0' '1' '2' '3' '4' '5' '6' '7' '8' '9']");
}

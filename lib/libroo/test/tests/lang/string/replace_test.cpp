#include "runtime_fixture.h"
#include <gtest/gtest.h>

using StringReplaceFunction = RooTest::RuntimeTestFixture;

TEST_F(StringReplaceFunction, replaces_all_literal_substrings)
{
  EXPECT_EQ(*runtime.eval(
              R"((roo.string/replace "Normal Club, Normal Shield" "Normal" "Enchanted"))"),
            *Roo::Value::string("Enchanted Club, Enchanted Shield"));
}

TEST_F(StringReplaceFunction, returns_original_when_search_is_missing)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace "Normal Club" "Sword" "Shield"))"),
            *Roo::Value::string("Normal Club"));
}

TEST_F(StringReplaceFunction, returns_original_when_search_is_empty)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace "Normal Club" "" "Enchanted"))"),
            *Roo::Value::string("Normal Club"));
}

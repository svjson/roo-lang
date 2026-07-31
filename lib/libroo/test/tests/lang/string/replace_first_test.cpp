#include "runtime_fixture.h"
#include <gtest/gtest.h>

using StringReplaceFirstFunction = RooTest::RuntimeTestFixture;

TEST_F(StringReplaceFirstFunction, replaces_first_literal_substring)
{
  EXPECT_EQ(
    *runtime.eval(
      R"((roo.string/replace-first "Normal Club, Normal Shield" "Normal" "Enchanted"))"),
    *Roo::Value::string("Enchanted Club, Normal Shield"));
}

TEST_F(StringReplaceFirstFunction, returns_original_when_search_is_missing)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace-first "Normal Club" "Sword" "Shield"))"),
            *Roo::Value::string("Normal Club"));
}

TEST_F(StringReplaceFirstFunction, returns_original_when_search_is_empty)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace-first "Normal Club" "" "Enchanted"))"),
            *Roo::Value::string("Normal Club"));
}

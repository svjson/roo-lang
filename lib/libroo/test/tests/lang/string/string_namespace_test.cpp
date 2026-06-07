#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using StringNamespace = RooTest::RuntimeTestFixture;

TEST_F(StringNamespace, replace_replaces_all_literal_substrings)
{
  EXPECT_EQ(*runtime.eval(
              R"((roo.string/replace "Normal Club, Normal Shield" "Normal" "Enchanted"))"),
            *Roo::Value::string("Enchanted Club, Enchanted Shield"));
}

TEST_F(StringNamespace, replace_first_replaces_first_literal_substring)
{
  EXPECT_EQ(
    *runtime.eval(
      R"((roo.string/replace-first "Normal Club, Normal Shield" "Normal" "Enchanted"))"),
    *Roo::Value::string("Enchanted Club, Normal Shield"));
}

TEST_F(StringNamespace, replace_returns_original_when_search_is_missing)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace "Normal Club" "Sword" "Shield"))"),
            *Roo::Value::string("Normal Club"));
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace-first "Normal Club" "Sword" "Shield"))"),
            *Roo::Value::string("Normal Club"));
}

TEST_F(StringNamespace, replace_returns_original_when_search_is_empty)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace "Normal Club" "" "Enchanted"))"),
            *Roo::Value::string("Normal Club"));
  EXPECT_EQ(*runtime.eval(R"((roo.string/replace-first "Normal Club" "" "Enchanted"))"),
            *Roo::Value::string("Normal Club"));
}

TEST_F(StringNamespace, can_be_required_with_alias)
{
  EXPECT_EQ(*runtime.eval(R"(
    (ns my-app.core (:require [roo.string :as string]))
    (string/replace "Normal Club" "Normal" "Enchanted")
  )"),
            *Roo::Value::string("Enchanted Club"));
}

TEST_F(StringNamespace, cannot_be_modified_from_roo)
{
  EXPECT_THROW(runtime.eval("(ns roo.string) (def x 1)"), Roo::NamespaceException);
}

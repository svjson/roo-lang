
#include "lisple/form.h"

#include <lisple/runtime.h>
#include <lisple/runtime/dict.h>

#include <gtest/gtest.h>

TEST(AssocFunction, add_key_to_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(*runtime.lookup_value(Lisple::Word("my-map")), *runtime.eval("{:a 1 :b 2}"));
}

TEST(AssocFunction, replace_key_in_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc my-map :a 10)");

  // Then
  EXPECT_EQ(result->to_string(), "{:a 10 :b 2}");
  EXPECT_EQ(*runtime.lookup_value(Lisple::Word("my-map")), *runtime.eval("{:a 1 :b 2}"));
}

TEST(AssocFunction, replace_key_in_map__retains_sibling_identities)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::sptr_rtval instance = runtime.eval(R"(
     (def my-map {:a {:name "Olle"} :b 100})
                                                )");
  // When
  Lisple::sptr_rtval assoc_result = runtime.eval("(assoc my-map :b 50)");

  // Then
  EXPECT_EQ(*assoc_result, *runtime.eval(R"({:a {:name "Olle"} :b 50})"));

  auto org_nested_obj = Lisple::Dict::get_property(instance, Lisple::RTValue::keyword("a"));
  auto mod_nested_obj =
    Lisple::Dict::get_property(assoc_result, Lisple::RTValue::keyword("a"));

  EXPECT_EQ(*org_nested_obj, *mod_nested_obj);
}

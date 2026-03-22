
#include <gtest/gtest.h>
#include <lisple/runtime.h>

#include "lisple/form.h"

TEST(AssocFunction, add_key_to_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(*runtime.lookup(Lisple::Word("my-map")), *runtime.eval("{:a 1 :b 2}"));
}

TEST(AssocFunction, replace_key_in_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc my-map :a 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 10 :b 2}"));
  EXPECT_EQ(*runtime.lookup(Lisple::Word("my-map")), *runtime.eval("{:a 1 :b 2}"));
}

TEST(AssocFunction, replace_key_in_map__retains_sibling_identities)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::sptr_sobject instance = runtime.eval(R"(
     (def my-map {:a {:name "Olle"} :b 100})
                                                )");
  // When
  Lisple::sptr_sobject assoc_result = runtime.eval("(assoc my-map :b 50)");

  // Then
  EXPECT_EQ(*assoc_result, *runtime.eval(R"({:a {:name "Olle"} :b 50})"));

  Lisple::sptr_sobject org_nested_obj = instance->get_sptr_property(Lisple::Key("a"));
  Lisple::sptr_sobject mod_nested_obj = assoc_result->get_sptr_property(Lisple::Key("a"));

  EXPECT_EQ(*org_nested_obj, *mod_nested_obj);

  if (auto* org_wrapped = dynamic_cast<Lisple::RuntimeValueWrapper*>(org_nested_obj.get()))
  {
    std::cout << "Nested Original is RuntimeValueWrapper" << std::endl;
    if (auto* mod_wrapped = dynamic_cast<Lisple::RuntimeValueWrapper*>(mod_nested_obj.get()))
    {
      std::cout << "Nested Modified is RuntimeValueWrapper" << std::endl;

      EXPECT_EQ(org_wrapped->val.get(), mod_wrapped->val.get());
    }
  }
  else
  {
    std::cout << "Nested Original is NOT RuntimeValueWrapper" << std::endl;
  }
}

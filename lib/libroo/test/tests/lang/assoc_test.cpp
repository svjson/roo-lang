
#include "roo/form.h"
#include "runtime_fixture.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <roo/exception.h>
#include <roo/runtime/dict.h>

using AssocFunction = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(AssocFunction, add_key_to_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b 2}"));
}

TEST_F(AssocFunction, replace_key_in_map)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc my-map :a 10)");

  // Then
  EXPECT_EQ(result->to_string(), "{:a 10 :b 2}");
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:a 1 :b 2}"));
}

TEST_F(AssocFunction, replace_key_when_same_value_occurs_before_key)
{
  // Given
  runtime.eval("(def my-map {:id :type :type :type/plain})");

  // When
  auto result = runtime.eval("(assoc my-map :type :type/updated)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:id :type :type :type/updated}"));
  EXPECT_EQ(*runtime.lookup("my-map"), *runtime.eval("{:id :type :type :type/plain}"));
}

TEST_F(AssocFunction, replace_key_in_map__retains_sibling_identities)
{
  // Given
  Roo::sptr_val instance = runtime.eval(R"(
     (def my-map {:a {:name "Olle"} :b 100})
                                                )");
  // When
  Roo::sptr_val assoc_result = runtime.eval("(assoc my-map :b 50)");

  // Then
  EXPECT_EQ(*assoc_result, *runtime.eval(R"({:a {:name "Olle"} :b 50})"));

  auto org_nested_obj = Roo::Dict::get_property(instance, Roo::Value::keyword("a"));
  auto mod_nested_obj = Roo::Dict::get_property(assoc_result, Roo::Value::keyword("a"));

  EXPECT_EQ(*org_nested_obj, *mod_nested_obj);
}

TEST_F(AssocFunction, add_and_replace_multiple)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc my-map :b 10 :c 3 :d \"some string\")");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}"));
  EXPECT_EQ(*runtime.lookup(*Roo::Value::symbol("my-map")), *runtime.eval("{:a 1 :b 2}"));
}

TEST_F(AssocFunction, throws_on_incomplete_key_value_chain)
{
  // Given
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When/Then
  EXPECT_THAT(
    [this]() { runtime.eval("(assoc my-map :b 10 :c)"); },
    ThrowsMessage<Roo::InvocationException>(HasSubstr("No value given for key ':c '")));
}

#include <map>
#include <string>

#include <roo/host/std_adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ContainsKeyPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(ContainsKeyPredicateFunction, finds_map_keys)
{
  EXPECT_EQ(*runtime.eval("(contains-key? {:a 1 :b 2} :b)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-key? {:a 1 :b 2} :missing)"),
            *Roo::Constant::BOOL_FALSE);
}

TEST_F(ContainsKeyPredicateFunction, does_not_match_map_values)
{
  EXPECT_EQ(*runtime.eval("(contains-key? {:a :b} :b)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(ContainsKeyPredicateFunction, finds_keys_with_nil_values)
{
  EXPECT_EQ(*runtime.eval("(contains-key? {:a nil} :a)"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(ContainsKeyPredicateFunction, treats_nil_as_an_empty_map)
{
  EXPECT_EQ(*runtime.eval("(contains-key? nil :a)"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(ContainsKeyPredicateFunction, finds_native_map_keys)
{
  std::map<int, std::string> values = {{1, "one"}};
  runtime.get_current_namespace().store(
    "values",
    Roo::NativeStdMapAdapter<int, std::string>::make_ref(values));

  EXPECT_EQ(*runtime.eval("(contains-key? values 1)"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains-key? values 2)"), *Roo::Constant::BOOL_FALSE);
}

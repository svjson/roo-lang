#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MapPredicateFunction = RooTest::RuntimeTestFixture;

TEST_F(MapPredicateFunction, maps_are_maps)
{
  EXPECT_EQ(*runtime.eval("(map? {:a 1})"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(map? {})"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(MapPredicateFunction, ordinary_values_are_not_maps)
{
  EXPECT_EQ(*runtime.eval("(map? nil)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(map? [1 2 3])"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(map? '(1 2 3))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval(R"((map? "abc"))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(map? 42)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(map? :name)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(map? true)"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(map? (fn [x] x))"), *Roo::Constant::BOOL_FALSE);
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using TypeOfFunction = RooTest::RuntimeTestFixture;

TEST_F(TypeOfFunction, returns_type_as_string)
{
  EXPECT_EQ(*runtime.eval("(type-of 42)"), *Roo::Value::string("number"));
  EXPECT_EQ(*runtime.eval("(type-of \"hello\")"), *Roo::Value::string("string"));
  EXPECT_EQ(*runtime.eval("(type-of 'hello)"), *Roo::Value::string("symbol"));
  EXPECT_EQ(*runtime.eval("(type-of :hello)"), *Roo::Value::string("keyword"));
  EXPECT_EQ(*runtime.eval("(type-of [1 2 3])"), *Roo::Value::string("vector"));
  EXPECT_EQ(*runtime.eval("(type-of {:a 1})"), *Roo::Value::string("map"));
  EXPECT_EQ(*runtime.eval("(type-of true)"), *Roo::Value::string("boolean"));
  EXPECT_EQ(*runtime.eval("(type-of false)"), *Roo::Value::string("boolean"));
  EXPECT_EQ(*runtime.eval("(type-of nil)"), *Roo::Value::string("nil"));
  EXPECT_EQ(*runtime.eval("(type-of (fn [x] x))"), *Roo::Value::string("executable"));
}

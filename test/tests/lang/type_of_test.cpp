#include "runtime_fixture.h"
#include <gtest/gtest.h>

using TypeOfFunction = LispleTest::RuntimeTestFixture;

TEST_F(TypeOfFunction, returns_type_as_string)
{
  EXPECT_EQ(*runtime.eval("(type-of 42)"), *Lisple::Value::string("number"));
  EXPECT_EQ(*runtime.eval("(type-of \"hello\")"), *Lisple::Value::string("string"));
  EXPECT_EQ(*runtime.eval("(type-of 'hello)"), *Lisple::Value::string("symbol"));
  EXPECT_EQ(*runtime.eval("(type-of :hello)"), *Lisple::Value::string("keyword"));
  EXPECT_EQ(*runtime.eval("(type-of [1 2 3])"), *Lisple::Value::string("vector"));
  EXPECT_EQ(*runtime.eval("(type-of {:a 1})"), *Lisple::Value::string("map"));
  EXPECT_EQ(*runtime.eval("(type-of true)"), *Lisple::Value::string("boolean"));
  EXPECT_EQ(*runtime.eval("(type-of false)"), *Lisple::Value::string("boolean"));
  EXPECT_EQ(*runtime.eval("(type-of nil)"), *Lisple::Value::string("nil"));
  EXPECT_EQ(*runtime.eval("(type-of (fn [x] x))"), *Lisple::Value::string("executable"));
}

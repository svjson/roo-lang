
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NameFunction = LispleTest::RuntimeTestFixture;
TEST_F(NameFunction, extract_name)
{
  // Given
  EXPECT_EQ(*runtime.eval("(name :accept/ok)"), *Lisple::Value::string("ok"));
  EXPECT_EQ(*runtime.eval("(name :ok)"), *Lisple::Value::string("ok"));
  EXPECT_EQ(*runtime.eval("(name 'my-app/some-function)"),
            *Lisple::Value::string("some-function"));
  EXPECT_EQ(*runtime.eval("(name 'some-function)"), *Lisple::Value::string("some-function"));
}


#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NameFunction = RooTest::RuntimeTestFixture;
TEST_F(NameFunction, extract_name)
{
  // Given
  EXPECT_EQ(*runtime.eval("(name :accept/ok)"), *Roo::Value::string("ok"));
  EXPECT_EQ(*runtime.eval("(name :ok)"), *Roo::Value::string("ok"));
  EXPECT_EQ(*runtime.eval("(name 'my-app/some-function)"),
            *Roo::Value::string("some-function"));
  EXPECT_EQ(*runtime.eval("(name 'some-function)"), *Roo::Value::string("some-function"));
}

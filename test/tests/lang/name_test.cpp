
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using NameFunction = LispleTest::RuntimeTestFixture;
TEST_F(NameFunction, extract_name)
{
  // Given
  EXPECT_EQ(*runtime.eval("(name :accept/ok)"), *Lisple::RTValue::string("ok"));
  EXPECT_EQ(*runtime.eval("(name :ok)"), *Lisple::RTValue::string("ok"));
  EXPECT_EQ(*runtime.eval("(name 'my-app/some-function)"),
            *Lisple::RTValue::string("some-function"));
  EXPECT_EQ(*runtime.eval("(name 'some-function)"),
            *Lisple::RTValue::string("some-function"));
}

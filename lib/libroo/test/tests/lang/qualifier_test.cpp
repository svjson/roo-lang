#include "runtime_fixture.h"
#include <gtest/gtest.h>

using QualifierFunction = RooTest::RuntimeTestFixture;
TEST_F(QualifierFunction, extract_qualifier)
{
  // Given
  EXPECT_EQ(*runtime.eval("(qualifier :accept/ok)"), *Roo::Value::string("accept"));
  EXPECT_EQ(*runtime.eval("(qualifier :ok)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(qualifier 'my-app/some-function)"),
            *Roo::Value::string("my-app"));
  EXPECT_EQ(*runtime.eval("(qualifier 'some-function)"), *Roo::Constant::NIL);
}

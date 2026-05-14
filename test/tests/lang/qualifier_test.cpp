#include "runtime_fixture.h"

#include <gtest/gtest.h>


using QualifierFunction = LispleTest::RuntimeTestFixture;
TEST_F(QualifierFunction, extract_qualifier)
{
  // Given
  EXPECT_EQ(*runtime.eval("(qualifier :accept/ok)"), *Lisple::RTValue::string("accept"));
  EXPECT_EQ(*runtime.eval("(qualifier :ok)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(qualifier 'my-app/some-function)"),
            *Lisple::RTValue::string("my-app"));
  EXPECT_EQ(*runtime.eval("(qualifier 'some-function)"), *Lisple::Constant::NIL);
}

#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(QualifierFunction, extract_qualifier)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(qualifier :accept/ok)"), *Lisple::RTValue::string("accept"));
  EXPECT_EQ(*runtime.eval("(qualifier :ok)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(qualifier 'my-app/some-function)"),
            *Lisple::RTValue::string("my-app"));
  EXPECT_EQ(*runtime.eval("(qualifier 'some-function)"), *Lisple::Constant::NIL);
}

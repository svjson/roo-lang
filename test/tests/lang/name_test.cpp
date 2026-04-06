
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(NameFunction, extract_name)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(name :accept/ok)"), *Lisple::RTValue::string("ok"));
  EXPECT_EQ(*runtime.eval("(name :ok)"), *Lisple::RTValue::string("ok"));
  EXPECT_EQ(*runtime.eval("(name 'my-app/some-function)"),
            *Lisple::RTValue::string("some-function"));
  EXPECT_EQ(*runtime.eval("(name 'some-function)"),
            *Lisple::RTValue::string("some-function"));
}

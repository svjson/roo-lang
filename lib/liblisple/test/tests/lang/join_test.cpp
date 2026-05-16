
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using JoinFunction = LispleTest::RuntimeTestFixture;
TEST_F(JoinFunction, join_strs)
{
  // Given
  EXPECT_EQ(*runtime.eval(R"((join " " "This" "is" "bat" "country"))"),
            *Lisple::Value::string("This is bat country"));
  EXPECT_EQ(*runtime.eval(R"((join "-" "hyphenate" "all" "the" "things"))"),
            *Lisple::Value::string("hyphenate-all-the-things"));
  EXPECT_EQ(*runtime.eval(R"((join ", " "CSV" "to" "the" "rescue"))"),
            *Lisple::Value::string("CSV, to, the, rescue"));
  EXPECT_EQ(*runtime.eval(R"((join "-"))"), *Lisple::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((join "-" "foreveralone"))"),
            *Lisple::Value::string("foreveralone"));
}

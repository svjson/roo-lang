
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using JoinFunction = RooTest::RuntimeTestFixture;
TEST_F(JoinFunction, join_strs)
{
  // Given
  EXPECT_EQ(*runtime.eval(R"((join " " "This" "is" "bat" "country"))"),
            *Roo::Value::string("This is bat country"));
  EXPECT_EQ(*runtime.eval(R"((join "-" "hyphenate" "all" "the" "things"))"),
            *Roo::Value::string("hyphenate-all-the-things"));
  EXPECT_EQ(*runtime.eval(R"((join ", " "CSV" "to" "the" "rescue"))"),
            *Roo::Value::string("CSV, to, the, rescue"));
  EXPECT_EQ(*runtime.eval(R"((join "-"))"), *Roo::Value::string(""));
  EXPECT_EQ(*runtime.eval(R"((join "-" "foreveralone"))"),
            *Roo::Value::string("foreveralone"));
}

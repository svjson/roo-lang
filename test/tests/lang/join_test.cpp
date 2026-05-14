
#include "runtime_fixture.h"

#include <gtest/gtest.h>


using JoinFunction = LispleTest::RuntimeTestFixture;
TEST_F(JoinFunction, join_strs)
{
  // Given
  EXPECT_EQ(*runtime.eval(R"((join " " "This" "is" "bat" "country"))"),
            *Lisple::RTValue::string("This is bat country"));
  EXPECT_EQ(*runtime.eval(R"((join "-" "hyphenate" "all" "the" "things"))"),
            *Lisple::RTValue::string("hyphenate-all-the-things"));
  EXPECT_EQ(*runtime.eval(R"((join ", " "CSV" "to" "the" "rescue"))"),
            *Lisple::RTValue::string("CSV, to, the, rescue"));
  EXPECT_EQ(*runtime.eval(R"((join "-"))"), *Lisple::RTValue::string(""));
  EXPECT_EQ(*runtime.eval(R"((join "-" "foreveralone"))"),
            *Lisple::RTValue::string("foreveralone"));
}


#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(JoinFunction, join_strs)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(*fixture.runtime.eval(R"((join " " "This" "is" "bat" "country"))"),
            *Lisple::RTValue::string("This is bat country"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-" "hyphenate" "all" "the" "things"))"),
            *Lisple::RTValue::string("hyphenate-all-the-things"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join ", " "CSV" "to" "the" "rescue"))"),
            *Lisple::RTValue::string("CSV, to, the, rescue"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-"))"), *Lisple::RTValue::string(""));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-" "foreveralone"))"),
            *Lisple::RTValue::string("foreveralone"));
}

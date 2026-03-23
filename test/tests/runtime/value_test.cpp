
#include <gtest/gtest.h>
#include <lisple/runtime/value.h>

TEST(RTValue_keyword, equals)
{
  auto kind = Lisple::RTValue::keyword("kind");
  auto kind_ = Lisple::RTValue::keyword("kind");
  auto value = Lisple::RTValue::keyword("value");

  EXPECT_TRUE(*kind == *kind);
  EXPECT_TRUE(*kind == *kind_);
  EXPECT_FALSE(*kind == *value);
  EXPECT_FALSE(*value == *kind);
}


#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

TEST(Value_keyword, equals)
{
  auto kind = Lisple::Value::keyword("kind");
  auto kind_ = Lisple::Value::keyword("kind");
  auto value = Lisple::Value::keyword("value");

  EXPECT_TRUE(*kind == *kind);
  EXPECT_TRUE(*kind == *kind_);
  EXPECT_FALSE(*kind == *value);
  EXPECT_FALSE(*value == *kind);
}

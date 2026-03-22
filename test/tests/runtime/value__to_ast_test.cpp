#include <gtest/gtest.h>
#include <lisple/form.h>
#include <lisple/runtime/value.h>

TEST(to_AST, boolean)
{
  EXPECT_EQ(Lisple::to_AST(*Lisple::Constant::TRUE), Lisple::B_TRUE);
  EXPECT_EQ(Lisple::to_AST(*Lisple::Constant::FALSE), Lisple::B_FALSE);
}

TEST(to_AST, string)
{
  EXPECT_EQ(*Lisple::to_AST(*Lisple::RTValue::string("test-string")),
            *Lisple::String::make("test-string"));
}

#include <lisple/form.h>
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

TEST(to_AST, boolean)
{
  EXPECT_EQ(Lisple::to_AST(*Lisple::Constant::BOOL_TRUE), Lisple::AST::B_TRUE);
  EXPECT_EQ(Lisple::to_AST(*Lisple::Constant::BOOL_FALSE), Lisple::AST::B_FALSE);
}

TEST(to_AST, string)
{
  EXPECT_EQ(*Lisple::to_AST(*Lisple::RTValue::string("test-string")),
            *Lisple::AST::String::make("test-string"));
}

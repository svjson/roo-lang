#include <gtest/gtest.h>
#include <roo/form.h>
#include <roo/runtime/value.h>

TEST(to_AST, boolean)
{
  EXPECT_EQ(Roo::to_AST(*Roo::Constant::BOOL_TRUE), Roo::AST::B_TRUE);
  EXPECT_EQ(Roo::to_AST(*Roo::Constant::BOOL_FALSE), Roo::AST::B_FALSE);
}

TEST(to_AST, string)
{
  EXPECT_EQ(*Roo::to_AST(*Roo::Value::string("test-string")),
            *Roo::AST::String::make("test-string"));
}

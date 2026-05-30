#include <roo/form.h>
#include <roo/runtime/value.h>

#include <gtest/gtest.h>

TEST(to_rt_value, boolean)
{
  Roo::sptr_ast_node t = Roo::AST::B_TRUE;
  Roo::sptr_ast_node f = Roo::AST::B_FALSE;

  EXPECT_EQ(Roo::to_rt_value(t), Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(Roo::to_rt_value(f), Roo::Constant::BOOL_FALSE);
}

TEST(to_rt_value, string)
{
  Roo::sptr_ast_node test_string = Roo::AST::String::make("test-string");

  EXPECT_EQ(*Roo::to_rt_value(test_string), *Roo::Value::string("test-string"));
}

TEST(to_rt_value, const_string)
{
  Roo::sptr_ast_node test_string = Roo::AST::String::make("test-string");

  EXPECT_EQ(*Roo::to_rt_value(*test_string), *Roo::Value::string("test-string"));
}

TEST(to_rt_value, vector_with_string_child)
{
  Roo::sptr_ast_node_v ast_elements = {Roo::AST::String::make("first"),
                                       Roo::AST::String::make("second")};
  Roo::sptr_ast_node ast_vector = Roo::AST::Vector::make(std::move(ast_elements));

  Roo::sptr_val_v rt_elements = {Roo::Value::string("first"), Roo::Value::string("second")};

  EXPECT_EQ(*Roo::to_rt_value(ast_vector), *Roo::Value::vector(rt_elements));
}

TEST(to_rt_value, map_with_string_value)
{
  Roo::sptr_ast_node_v ast_elements = {Roo::AST::Keyword::make("name"),
                                       Roo::AST::String::make("Swedish Lion")};
  Roo::sptr_ast_node ast_map = Roo::AST::Map::make(std::move(ast_elements));

  Roo::sptr_val_v rt_elements = {Roo::Value::keyword("name"),
                                 Roo::Value::string("Swedish Lion")};

  EXPECT_EQ(*Roo::to_rt_value(ast_map), *Roo::Value::map(rt_elements));
}

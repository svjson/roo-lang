#include <lisple/form.h>
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

TEST(to_rt_value, boolean)
{
  Lisple::sptr_sobject t = Lisple::AST::B_TRUE;
  Lisple::sptr_sobject f = Lisple::AST::B_FALSE;

  EXPECT_EQ(Lisple::to_rt_value(t), Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(Lisple::to_rt_value(f), Lisple::Constant::BOOL_FALSE);
}

TEST(to_rt_value, string)
{
  Lisple::sptr_sobject test_string = Lisple::AST::String::make("test-string");

  EXPECT_EQ(*Lisple::to_rt_value(test_string), *Lisple::Value::string("test-string"));
}

TEST(to_rt_value, const_string)
{
  Lisple::sptr_sobject test_string = Lisple::AST::String::make("test-string");

  EXPECT_EQ(*Lisple::to_rt_value(*test_string), *Lisple::Value::string("test-string"));
}

TEST(to_rt_value, vector_with_string_child)
{
  Lisple::sptr_sobject_v ast_elements = {Lisple::AST::String::make("first"),
                                         Lisple::AST::String::make("second")};
  Lisple::sptr_sobject ast_vector = Lisple::AST::Vector::make(std::move(ast_elements));

  Lisple::sptr_val_v rt_elements = {Lisple::Value::string("first"),
                                    Lisple::Value::string("second")};

  EXPECT_EQ(*Lisple::to_rt_value(ast_vector), *Lisple::Value::vector(rt_elements));
}

TEST(to_rt_value, map_with_string_value)
{
  Lisple::sptr_sobject_v ast_elements = {Lisple::AST::Keyword::make("name"),
                                         Lisple::AST::String::make("Swedish Lion")};
  Lisple::sptr_sobject ast_map = Lisple::AST::Map::make(std::move(ast_elements));

  Lisple::sptr_val_v rt_elements = {Lisple::Value::keyword("name"),
                                    Lisple::Value::string("Swedish Lion")};

  EXPECT_EQ(*Lisple::to_rt_value(ast_map), *Lisple::Value::map(rt_elements));
}

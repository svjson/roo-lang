#include <lisple/form.h>
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

TEST(to_rt_value, boolean)
{
  Lisple::sptr_sobject t = Lisple::B_TRUE;
  Lisple::sptr_sobject f = Lisple::B_FALSE;

  EXPECT_EQ(Lisple::to_rt_value(t), Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(Lisple::to_rt_value(f), Lisple::Constant::BOOL_FALSE);
}

TEST(to_rt_value, string)
{
  Lisple::sptr_sobject test_string = Lisple::String::make("test-string");

  EXPECT_EQ(*Lisple::to_rt_value(test_string), *Lisple::RTValue::string("test-string"));
}

TEST(to_rt_value, const_string)
{
  Lisple::sptr_sobject test_string = Lisple::String::make("test-string");

  EXPECT_EQ(*Lisple::to_rt_value(*test_string), *Lisple::RTValue::string("test-string"));
}

TEST(to_rt_value, vector_with_string_child)
{
  Lisple::sptr_sobject_v ast_elements = {Lisple::String::make("first"),
                                         Lisple::String::make("second")};
  Lisple::sptr_sobject ast_vector = Lisple::Vector::make(std::move(ast_elements));

  Lisple::sptr_rtval_v rt_elements = {Lisple::RTValue::string("first"),
                                      Lisple::RTValue::string("second")};

  EXPECT_EQ(*Lisple::to_rt_value(ast_vector), *Lisple::RTValue::vector(rt_elements));
}

TEST(to_rt_value, map_with_string_value)
{
  Lisple::sptr_sobject_v ast_elements = {Lisple::Keyword::make("name"),
                                         Lisple::String::make("Swedish Lion")};
  Lisple::sptr_sobject ast_map = Lisple::Map::make(std::move(ast_elements));

  Lisple::sptr_rtval_v rt_elements = {Lisple::RTValue::keyword("name"),
                                      Lisple::RTValue::string("Swedish Lion")};

  EXPECT_EQ(*Lisple::to_rt_value(ast_map), *Lisple::RTValue::map(rt_elements));
}

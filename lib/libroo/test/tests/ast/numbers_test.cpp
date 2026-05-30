
#include <string>

#include <gtest/gtest.h>
#include <roo/form.h>

TEST(Number, from_string)
{
  const std::string NUM1_STR = "123456789";
  const std::string NUM2_STR = "1103515245";
  const std::string NUM3_STR = "2147483648";
  auto num1 = Roo::AST::Number::make(NUM1_STR);
  auto num2 = Roo::AST::Number::make(NUM2_STR);
  auto num3 = Roo::AST::Number::make(NUM3_STR);

  EXPECT_EQ(num1->to_string(), NUM1_STR);
  EXPECT_EQ(num2->to_string(), NUM2_STR);
  EXPECT_EQ(num3->to_string(), NUM3_STR);
}

TEST(Number, equality)
{
  // Given
  Roo::AST::Number num1(25);
  Roo::AST::Number num2(25);
  Roo::AST::String str("25");
  Roo::AST::Number num3(28);

  EXPECT_EQ(num1, num2);
  EXPECT_EQ(num2, num1);
  EXPECT_NE(num1, str);
  EXPECT_NE(num1, num3);
}

TEST(Number, int_value)
{
  Roo::AST::Number num1(0);
  Roo::AST::Number num2(25);
  Roo::AST::Number num3(40);
  Roo::AST::Number num4(4000);

  EXPECT_EQ(num1.int_value(), 0);
  EXPECT_EQ(num2.int_value(), 25);
  EXPECT_EQ(num3.int_value(), 40);
  EXPECT_EQ(num4.int_value(), 4000);
}

TEST(Number, num_type_on_construction)
{
  ASSERT_TRUE(Roo::AST::Number(1).is_num_type(Roo::AST::NumberType::INT));
  ASSERT_TRUE(Roo::AST::Number(-1).is_num_type(Roo::AST::NumberType::INT));
  ASSERT_TRUE(Roo::AST::Number(45.0f).is_num_type(Roo::AST::NumberType::FLOAT));
  ASSERT_TRUE(Roo::AST::Number(1.0f).is_num_type(Roo::AST::NumberType::FLOAT));
  ASSERT_TRUE(Roo::AST::Number::make("1")->is_num_type(Roo::AST::NumberType::INT));
  ASSERT_TRUE(Roo::AST::Number::make("1.0")->is_num_type(Roo::AST::NumberType::FLOAT));
}

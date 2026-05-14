#include <lisple/form.h>
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

TEST(is_truthy, boolean)
{
  EXPECT_TRUE(Lisple::is_truthy(*Lisple::Constant::BOOL_TRUE));
  EXPECT_FALSE(Lisple::is_truthy(*Lisple::Constant::BOOL_FALSE));
}

TEST(is_truthy, number)
{
  for (int i = -1000; i <= 1000; i++)
  {
    EXPECT_TRUE(Lisple::is_truthy(*Lisple::Value::number(i)));
  }
}

TEST(is_truthy, nil)
{
  EXPECT_FALSE(Lisple::is_truthy(*Lisple::Constant::NIL));
}

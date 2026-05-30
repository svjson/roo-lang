#include <gtest/gtest.h>
#include <roo/form.h>
#include <roo/runtime/value.h>

TEST(is_truthy, boolean)
{
  EXPECT_TRUE(Roo::is_truthy(*Roo::Constant::BOOL_TRUE));
  EXPECT_FALSE(Roo::is_truthy(*Roo::Constant::BOOL_FALSE));
}

TEST(is_truthy, number)
{
  for (int i = -1000; i <= 1000; i++)
  {
    EXPECT_TRUE(Roo::is_truthy(*Roo::Value::number(i)));
  }
}

TEST(is_truthy, nil)
{
  EXPECT_FALSE(Roo::is_truthy(*Roo::Constant::NIL));
}

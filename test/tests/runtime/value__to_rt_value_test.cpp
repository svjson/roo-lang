#include <gtest/gtest.h>
#include <lisple/form.h>
#include <lisple/runtime/value.h>

TEST(to_rt_value, boolean)
{
  Lisple::sptr_sobject t = Lisple::B_TRUE;
  Lisple::sptr_sobject f = Lisple::B_FALSE;

  EXPECT_EQ(Lisple::to_rt_value(t), Lisple::Constant::TRUE);
  EXPECT_EQ(Lisple::to_rt_value(f), Lisple::Constant::FALSE);
}

TEST(to_rt_value, string)
{
  Lisple::sptr_sobject test_string = Lisple::String::make("test-string");

  EXPECT_EQ(*Lisple::to_rt_value(test_string), *Lisple::RTValue::string("test-string"));
}

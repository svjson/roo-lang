#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NotEmptyPFunction = RooTest::RuntimeTestFixture;

TEST_F(NotEmptyPFunction, not_emptyp_seqs)
{
  EXPECT_EQ(*runtime.eval("(not-empty? '(\"value1\" \"value2\"))"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not-empty? [\"value1\" \"value2\"])"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not-empty? {:key1 \"value1\" :key2 \"value2\"})"),
            *Roo::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(not-empty? '())"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not-empty? [])"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(not-empty? {})"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(NotEmptyPFunction, not_emptyp_strings)
{
  EXPECT_EQ(*runtime.eval("(not-empty? \"a string\")"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not-empty? \"a\")"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not-empty? \" \")"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(not-empty? \"\")"), *Roo::Constant::BOOL_FALSE);
}

TEST_F(NotEmptyPFunction, nil_is_empty)
{
  EXPECT_EQ(*runtime.eval("(not-empty? nil)"), *Roo::Constant::BOOL_FALSE);
}

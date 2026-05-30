#include "runtime_fixture.h"
#include <gtest/gtest.h>

using EmptyPFunction = RooTest::RuntimeTestFixture;
TEST_F(EmptyPFunction, emptyp_seqs)
{
  // Given
  EXPECT_EQ(*runtime.eval("(empty? '(\"value1\" \"value2\"))"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(empty? [\"value1\" \"value2\"])"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(empty? {:key1 \"value1\" :key2 \"value2\"})"),
            *Roo::Constant::BOOL_FALSE);

  EXPECT_EQ(*runtime.eval("(empty? '())"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(empty? [])"), *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(empty? {})"), *Roo::Constant::BOOL_TRUE);
}

TEST_F(EmptyPFunction, emptyp_strings)
{
  // Given
  EXPECT_EQ(*runtime.eval("(empty? \"a string\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(empty? \"a\")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(empty? \" \")"), *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(empty? \"\")"), *Roo::Constant::BOOL_TRUE);
}

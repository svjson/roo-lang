
#include <roo/runtime/eval_plan.h>

#include <gtest/gtest.h>

TEST(EvalPattern, get_repeating)
{
  // Given
  Roo::EvalPattern pattern({&Roo::Eval::LITERAL, &Roo::Eval::POSTPONE}, true);

  // Then
  EXPECT_EQ(pattern.get(0), &Roo::Eval::LITERAL);
  EXPECT_EQ(pattern.get(1), &Roo::Eval::POSTPONE);
  EXPECT_EQ(pattern.get(2), &Roo::Eval::LITERAL);
  EXPECT_EQ(pattern.get(3), &Roo::Eval::POSTPONE);
  EXPECT_EQ(pattern.get(4), &Roo::Eval::LITERAL);
  EXPECT_EQ(pattern.get(5), &Roo::Eval::POSTPONE);
}

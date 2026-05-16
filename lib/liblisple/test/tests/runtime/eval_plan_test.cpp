
#include <gtest/gtest.h>
#include <lisple/runtime/eval_plan.h>

TEST(EvalPattern, get_repeating)
{
  // Given
  Lisple::EvalPattern pattern({&Lisple::Eval::LITERAL, &Lisple::Eval::POSTPONE}, true);

  // Then
  EXPECT_EQ(pattern.get(0), &Lisple::Eval::LITERAL);
  EXPECT_EQ(pattern.get(1), &Lisple::Eval::POSTPONE);
  EXPECT_EQ(pattern.get(2), &Lisple::Eval::LITERAL);
  EXPECT_EQ(pattern.get(3), &Lisple::Eval::POSTPONE);
  EXPECT_EQ(pattern.get(4), &Lisple::Eval::LITERAL);
  EXPECT_EQ(pattern.get(5), &Lisple::Eval::POSTPONE);
}

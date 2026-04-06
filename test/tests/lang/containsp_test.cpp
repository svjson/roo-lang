#include <lisple/adapter.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ======================================================================
 * ContainsPredicateFunction - (contains? [...] predicate)
 * ======================================================================
 */

TEST(ContainsPredicateFunction, contains_array)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-vec [1 3 5 6 7 8])");

  // Then
  EXPECT_EQ(*runtime.eval("(contains? my-vec 1)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 2)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 3)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 4)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 5)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 6)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 7)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 8)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 9)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 10)"), *Lisple::Constant::BOOL_FALSE);
}

TEST(ContainsPredicateFunction, contains_vector_int)
{
  // Given
  Lisple::Runtime runtime;
  std::vector<int> int_v{1, 3, 5, 6, 7, 8};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 1)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 2)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 3)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 4)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 5)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 6)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 7)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 8)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 9)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 10)"), *Lisple::Constant::BOOL_FALSE);
}

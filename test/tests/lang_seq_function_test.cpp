
#include <memory>
#include <vector>

#include <lisple/adapter.h>
#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>
#include <lisple/type.h>

#include <ext/alloc_traits.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace ::testing;

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

/*
 * ===================================
 * RandNthFunction - (rand-nth [...])
 * ===================================
 */

TEST(RandNth, all_elements_possible)
{
  // Given
  Lisple::Runtime runtime;

  std::vector freq{0, 0, 0, 0, 0};

  // When
  for (int i = 0; i < 5000; i++)
  {
    int num = runtime.eval("(rand-nth [0 1 2 3 4])")->i64();
    freq[num]++;
    ;
  }

  // Then
  for (int num_freq : freq)
  {
    EXPECT_NE(num_freq, 0);
  }
}

TEST(RandNth, all_elements_possible_wrapped)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v{0, 1, 2, 3, 4};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  std::vector freq{0, 0, 0, 0, 0};

  // When
  for (int i = 0; i < 5000; i++)
  {
    int num = runtime.eval("(rand-nth wrapped-vec)")->i64();
    freq[num]++;
    ;
  }

  // Then
  for (int num_freq : freq)
  {
    EXPECT_NE(num_freq, 0);
  }
}

TEST(RandNth, single_element)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  for (int i = 0; i < 1000; i++)
  {
    int num = runtime.eval("(rand-nth [8])")->i64();
    EXPECT_EQ(num, 8);
  }
}

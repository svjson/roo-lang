
#include "gmock/gmock.h"
#include <gtest/gtest.h>

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <ext/alloc_traits.h>
#include <memory>
#include <vector>

#include <lisple/adapter.h>
#include <lisple/runtime.h>
#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/type.h>

#include "runtime_fixture.h"
#include "test_host_objects.h"

using namespace ::testing;

/*
 * ======================================================================
 * ConcatFunction - (concat [...] 5)
 * ConcatFunction - (concat [...] [...])
 * ======================================================================
 */

TEST(ConcatFunction, numbers)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(concat [1 2 3] [4 5 6])");

  // Then
  EXPECT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6]"));
}

TEST(ConcatFunction, number_to_vector_int)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v { 1, 2, 3 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(concat wrapped-vec 4)");

  // Then
  EXPECT_EQ(*retval, *runtime.eval("[1 2 3 4]"));
  EXPECT_EQ(runtime.lookup("wrapped-vec")->to_string(), runtime.eval("[1 2 3]")->to_string());
  EXPECT_THAT(int_v, ElementsAre(1, 2, 3));
}

TEST(ConcatFunction, host_objects_and_primitives)
{
  // Given
  Lisple::Runtime runtime;

  runtime.get_current_namespace().store("cessna", Tests::VehicleAdapter::make<Tests::Vehicle>("Cessna", 2));

  // When
  auto retval = runtime.eval("(concat [] cessna {:a 2} \"rawk n rawl\")");

  EXPECT_EQ(retval->to_string(), R"([{:model-name "Cessna" :seats 2} {:a 2} "rawk n rawl"])");
}

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
  EXPECT_EQ(*runtime.eval("(contains? my-vec 1)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 2)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 3)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 4)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 5)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 6)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 7)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 8)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 9)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? my-vec 10)"), *Lisple::B_FALSE);
}

TEST(ContainsPredicateFunction, contains_vector_int)
{
  // Given
  Lisple::Runtime runtime;
  std::vector<int> int_v { 1, 3, 5, 6, 7, 8 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 1)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 2)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 3)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 4)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 5)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 6)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 7)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 8)"), *Lisple::B_TRUE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 9)"), *Lisple::B_FALSE);
  EXPECT_EQ(*runtime.eval("(contains? wrapped-vec 10)"), *Lisple::B_FALSE);
}

/*
 * ===================================
 * CountFunction - (count [...])
 *               - (count "a string")
 * ===================================
 */

TEST(CountFunction, count_seq_length)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v { 50, 100, 90 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(*runtime.eval("(count [])"), Lisple::Number(0));
  EXPECT_EQ(*runtime.eval("(count ['a' 'b'])"), Lisple::Number(2));
  EXPECT_EQ(*runtime.eval("(count [1 2 3])"), Lisple::Number(3));
  EXPECT_EQ(*runtime.eval("(count [0 0 0 0 0])"), Lisple::Number(5));
  EXPECT_EQ(*runtime.eval("(count wrapped-vec)"), Lisple::Number(3));
}

TEST(CountFunction, count_str_length)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(count \"\")"), Lisple::Number(0));
  EXPECT_EQ(*runtime.eval("(count \" \")"), Lisple::Number(1));
  EXPECT_EQ(*runtime.eval("(count \"..\")"), Lisple::Number(2));
  EXPECT_EQ(*runtime.eval("(count \" !\")"), Lisple::Number(2));
  EXPECT_EQ(*runtime.eval("(count \"abc\")"), Lisple::Number(3));
  EXPECT_EQ(*runtime.eval("(count \"Where is my garmonbozia?\")"), Lisple::Number(24));
  EXPECT_EQ(*runtime.eval("(count \"0123456789\")"), Lisple::Number(10));
  EXPECT_EQ(*runtime.eval("(count {:a 1 :b 2})"), Lisple::Number(2));
}

/*
 * ===================================
 * FlattenFunction - (flatten [...] [...])
 * ===================================
 */

TEST(FlattenFunction, array_of_array)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(flatten [[1 2 3] [4 5 6] [7 8 9]])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST(FlattenFunction, nested_arrays)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(flatten [1 [2 3] [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST(FlattenFunction, nested_vector_int_in_array)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v { 2, 3 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(flatten [1 wrapped-vec [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6 7 8 9]"));
}

/*
 * ===================================
 * HeadFunction - (head [...])
 * ===================================
 */

TEST(HeadFunction, head_of_seq)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-array ['a' 'b' 'c'])");

  std::vector<int> int_v { 50, 100, 90 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result_ref = runtime.eval("(head my-array)");
  auto result_lit = runtime.eval("(head ['a' 'b' 'c'])");
  auto result_wrapped = runtime.eval("(head wrapped-vec)");

  // Then
  EXPECT_EQ(*result_ref, Lisple::Char('a'));
  EXPECT_EQ(*result_lit, Lisple::Char('a'));
  EXPECT_EQ(*result_wrapped, Lisple::Number(50)) << result_wrapped->to_string() << " vs " << Lisple::Number(50).to_string();
}

TEST(HeadFunction, single_element)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v { 9 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result_lit = runtime.eval("(head ['a'])");
  auto result_wrapped = runtime.eval("(head wrapped-vec)");

  // Then
  EXPECT_EQ(*result_lit, Lisple::Char('a'));
  EXPECT_EQ(*result_wrapped, Lisple::Number(9));
}

/*
 * ===================================
 * TailFunction - (tail [...])
 * ===================================
 */

TEST(TailFunction, tail_of_array)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-array ['a' 'b' 'c'])");

  std::vector<int> int_v { 50, 100, 90 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));


  // When
  auto result_ref = runtime.eval("(tail my-array)");
  auto result_lit = runtime.eval("(tail ['a' 'b' 'c'])");
  auto result_wrapped = runtime.eval("(tail wrapped-vec)");

  // Then
  EXPECT_EQ(*result_ref, *runtime.eval("['b' 'c']"));
  EXPECT_EQ(*result_lit, *runtime.eval("['b' 'c']"));
  EXPECT_EQ(*result_wrapped, *runtime.eval("[100 90]"));
}

TEST(TailFunction, single_element)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v { 50  };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result = runtime.eval("(tail ['a'])");
  auto result_wrapped = runtime.eval("(tail wrapped-vec)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("[]"));
  EXPECT_EQ(*result_wrapped, *runtime.eval("[]"));
}

/*
 * ===================================
 * LastFunction - (last [...])
 * ===================================
 */

TEST(LastFunction, last)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v { 50, 100, 90 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(*runtime.eval("(last [1 2 3])"), Lisple::Number(3));
  EXPECT_EQ(*runtime.eval("(last wrapped-vec)"), Lisple::Number(90));
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

  std::vector freq { 0, 0, 0, 0, 0 };

  // When
  for (int i=0; i < 5000; i++)
  {
    int num = runtime.eval("(rand-nth [0 1 2 3 4])")->as<Lisple::Number>().value;
    freq[num]++;;
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

  std::vector<int> int_v { 0, 1, 2, 3, 4 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  std::vector freq { 0, 0, 0, 0, 0 };

  // When
  for (int i=0; i < 5000; i++)
  {
    int num = runtime.eval("(rand-nth wrapped-vec)")->as<Lisple::Number>().value;
    freq[num]++;;
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
  for (int i=0; i<1000;i++)
  {
    int num = runtime.eval("(rand-nth [8])")->as<Lisple::Number>().value;
    EXPECT_EQ(num, 8);
  }
}

/*
 * ===================================
 * TakeFunction - (take [...])
 * ===================================
 */
TEST(TakeFunction, take)
{
  // Given
  Lisple::Runtime runtime;
  std::vector<int> int_v { 0, 1, 2, 3, 4 };
  runtime.get_current_namespace().store("wrapped-vec", std::make_shared<Lisple::VectorInt>(int_v));

  // Then
  EXPECT_EQ(runtime.eval("(take 2 [1 2 3 4 5 6 7 8 9 10])")->to_string(), "[1 2]");
  EXPECT_EQ(runtime.eval("(take 5 [1 2 3])")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.eval("(take 10 [])")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(take 0 [1 2 3])")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(take 3 wrapped-vec)")->to_string(), "[0 1 2]");
}

/*
 * ======================================================================
 * SeqMatchFunction - (seq-match [{...} {...}] pattern)
 * ======================================================================
 */

TEST(SeqMatchFunction, seq_match_single_field)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"((def my-seq [{:id 1 :name "Air Man"} {:id 2 :name "Bubble Man"}]))");

  // Then
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id 1})")->to_string(), R"({:id 1 :name "Air Man"})");
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id 2})")->to_string(), R"({:id 2 :name "Bubble Man"})");
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id 3})")->to_string(), R"(nil)");
}

TEST(SeqMatchFunction, seq_match_missing_field)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"((def my-seq [{:id 1 :name "Air Man"} {:id 2 :name "Bubble Man"}]))");

  // Then
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:electric-boogaloo 2})")->to_string(), "nil");
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id nil})")->to_string(), "nil");
}

TEST(SeqMatchFunction, seq_match_nested)
{
  // Given
  Lisple::Runtime runtime;
  const std::string air_man =
    R"({:id 1 :name "Air Man" :stats {:weapon "Air Shooter" :weakness "Leaf Shield"}})";
  const std::string bubble_man =
    R"({:id 2 :name "Bubble Man" :stats {:weapon "Bubble Lead" :weakness "Metal Blade"}})";
  const std::string wood_man =
    R"({:id 3 :name "Wood Man" :stats {:weapon "Leaf Shield" :weakness "Metal Blade"}})";

  // When
  runtime.eval("(def my-seq [" + air_man + bubble_man + wood_man + "])");

  // Then
  EXPECT_EQ(runtime.eval(R"((seq-match my-seq {:stats {:weakness "Metal Blade"}}))")->to_string(), bubble_man);
  EXPECT_EQ(runtime.eval(R"((seq-match my-seq {:stats {:weapon "Leaf Shield"}}))")->to_string(), wood_man);
  EXPECT_EQ(runtime.eval(R"((seq-match my-seq {:id 3 :stats {:weapon "Leaf Shield"}}))")->to_string(), wood_man);
  EXPECT_EQ(runtime.eval(R"((seq-match my-seq {:stats {:weakness "Bubble Lead"}}))")->to_string(), "nil");
}


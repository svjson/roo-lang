#include <memory>
#include <vector>

#include <lisple/adapter.h>
#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/runtime.h>
#include <lisple/runtime/seq.h>
#include <lisple/type.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <ext/alloc_traits.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace ::testing;

/*
 * ======================================================================
 * FilterFunction - (filter [...] predicate)
 * ======================================================================
 */

TEST(FilterFunction, filter_array)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(filter [1 2 3 4 5 6] even?)");

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*result));

  ASSERT_EQ(Lisple::count(*result), 3);

  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 4);
  EXPECT_EQ(result->elements().at(2)->i64(), 6);
}

TEST(FilterFunction, filter_vector_int)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v{1, 2, 3, 4, 5, 6};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto result = runtime.eval("(filter wrapped-vec even?)");

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*result));

  ASSERT_EQ(result->elements().size(), 3);

  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 4);
  EXPECT_EQ(result->elements().at(2)->i64(), 6);
}

/*
 * ======================================================================
 * FindFirstFunction - (find-first [...] predicate)
 * ======================================================================
 */

TEST(FindFirstFunction, find_first_array)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval(R"((def my-array ["AA" "BB" "CCC" "DDDD" "EEE" "FF"]))");

  // When
  auto three_letter =
    fixture.runtime.eval("(find-first my-array (fn [lmnt] (= 3 (count lmnt))))");
  auto four_letter =
    fixture.runtime.eval("(find-first my-array (fn [lmnt] (= 4 (count lmnt))))");
  auto five_letter =
    fixture.runtime.eval("(find-first my-array (fn [lmnt] (= 5 (count lmnt))))");

  // Then
  EXPECT_EQ(*three_letter, *Lisple::RTValue::string("CCC"));
  EXPECT_EQ(*four_letter, *Lisple::RTValue::string("DDDD"));
  EXPECT_EQ(*five_letter, *Lisple::Constant::NIL);
}

/*
 * ======================================================================
 * KeepFunction - (keep [...] select-transform-fm)
 * ======================================================================
 */

TEST(KeepFunction, transform_even)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result =
    runtime.eval("(keep [1 2 3 4] (fn [x] (when (even? x) (str \"Number \" x))))");

  // Then
  ASSERT_EQ(*result,
            *Lisple::RTValue::vector(
              {Lisple::RTValue::string("Number 2"), Lisple::RTValue::string("Number 4")}));
}

/*
 * ======================================================================
 * RemoveBangFunction - (remove! [1 2 3 4] predicate)
 * ======================================================================
 */

TEST(RemoveBangFunction, remove_even_from_array)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-seq [1 2 3 4])");

  // When
  runtime.eval("(remove! (fn [n] (even? n)) my-seq)");

  // Then
  auto my_seq = runtime.lookup_value("my-seq");

  EXPECT_EQ(Lisple::count(*my_seq), 2);
  EXPECT_EQ(my_seq->elements().at(0)->i64(), 1);
  EXPECT_EQ(my_seq->elements().at(1)->i64(), 3);
}

TEST(RemoveBangFunction, remove_even_from_vector_int)
{
  // Given
  Lisple::Runtime runtime;
  std::vector<int> int_v{1, 2, 3, 4};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  runtime.eval("(remove! (fn [n] (even? n)) wrapped-vec)");

  // Then
  auto my_seq = runtime.lookup_value("wrapped-vec");
  ASSERT_EQ(Lisple::count(*my_seq), 2);
  ASSERT_EQ(Lisple::get_children(*my_seq).at(0)->type, Lisple::RTValue::Type::NUMBER);
  EXPECT_EQ(Lisple::get_children(*my_seq).at(0)->i64(), 1);
  ASSERT_EQ(Lisple::get_children(*my_seq).at(1)->type, Lisple::RTValue::Type::NUMBER);
  EXPECT_EQ(Lisple::get_children(*my_seq).at(1)->i64(), 3);

  EXPECT_THAT(int_v, ElementsAre(1, 3));
}

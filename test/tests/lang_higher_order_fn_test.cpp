

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <ext/alloc_traits.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <lisple/adapter.h>
#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/runtime.h>
#include <lisple/type.h>
#include <memory>
#include <vector>

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
  auto retval = runtime.eval("(filter [1 2 3 4 5 6] even?)");

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*retval));

  Lisple::Object& result = retval->as<Lisple::Array>();
  ASSERT_EQ(result.get_children().size(), 3);

  EXPECT_EQ(*result.get_children().at(0), Lisple::Number(2));
  EXPECT_EQ(*result.get_children().at(1), Lisple::Number(4));
  EXPECT_EQ(*result.get_children().at(2), Lisple::Number(6));
}

TEST(FilterFunction, filter_vector_int)
{
  // Given
  Lisple::Runtime runtime;

  std::vector<int> int_v{1, 2, 3, 4, 5, 6};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(filter wrapped-vec even?)");

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*retval));

  Lisple::Object& result = retval->as<Lisple::Array>();
  ASSERT_EQ(result.get_children().size(), 3);

  EXPECT_EQ(*result.get_children().at(0), Lisple::Number(2));
  EXPECT_EQ(*result.get_children().at(1), Lisple::Number(4));
  EXPECT_EQ(*result.get_children().at(2), Lisple::Number(6));
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
  std::cout << "===============================================" << std::endl;
  auto three_letter =
    fixture.runtime.eval("(find-first my-array (fn [lmnt] (= 3 (count lmnt))))");
  std::cout << "===============================================" << std::endl;
  // auto four_letter =
  //   fixture.runtime.eval("(find-first my-array (fn [lmnt] (= 4 (count lmnt))))");
  // auto five_letter =
  //   fixture.runtime.eval("(find-first my-array (fn [lmnt] (= 5 (count lmnt))))");

  // Then
  EXPECT_EQ(*three_letter, Lisple::String("CCC"));
  // EXPECT_EQ(*four_letter, Lisple::String("DDDD"));
  // EXPECT_EQ(*five_letter, *Lisple::NIL);
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
  ASSERT_EQ(
    *result,
    Lisple::Array({Lisple::String::make("Number 2"), Lisple::String::make("Number 4")}));
}

/*
 * ======================================================================
 * ReduceKeyValueFunction - (reduce-kv [...] reducer-fn)
 * ======================================================================
 */

TEST(ReduceKeyValueFunction, recreate_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def original-map {:a [1 2 3 4 5] :b [1 2 3]})");

  // when
  auto retval =
    runtime.eval("(reduce-kv original-map {} (fn [r k v] (assoc r k (count v))))");

  // Then
  EXPECT_EQ(*retval, *runtime.eval("{:a 5 :b 3}"));
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
  Lisple::sptr_sobject my_seq = runtime.lookup("my-seq");

  EXPECT_EQ(my_seq->get_children().size(), 2);
  std::cout << "Got: " << my_seq->to_string() << std::endl;
  EXPECT_EQ(my_seq->get_children().at(0)->as<Lisple::Number>().int_value(), 1);
  EXPECT_EQ(my_seq->get_children().at(1)->as<Lisple::Number>().int_value(), 3);
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
  auto my_seq = runtime.lookup(Lisple::Word("wrapped-vec"));
  EXPECT_EQ(my_seq->get_children().size(), 2);
  EXPECT_EQ(my_seq->get_children().at(0)->as<Lisple::Number>().int_value(), 1);
  EXPECT_EQ(my_seq->get_children().at(1)->as<Lisple::Number>().int_value(), 3);

  EXPECT_THAT(int_v, ElementsAre(1, 3));
}

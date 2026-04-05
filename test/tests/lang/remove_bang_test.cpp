
#include <lisple/adapter.h>
#include <lisple/runtime.h>
#include <lisple/runtime/seq.h>

#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace ::testing;

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


#include <lisple/exception.h>
#include <lisple/runtime/seq.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using RemoveBangFunction = LispleTest::RuntimeTestFixture;

/*
 * ======================================================================
 * RemoveBangFunction - (remove! [1 2 3 4] predicate)
 * ======================================================================
 */

TEST_F(RemoveBangFunction, remove_even_from_vector)
{
  // Given
  runtime.eval("(def my-seq [1 2 3 4])");

  // When
  runtime.eval("(remove! (fn [n] (even? n)) my-seq)");

  // Then
  auto my_seq = runtime.lookup("my-seq");

  EXPECT_EQ(Lisple::count(*my_seq), 2);
  EXPECT_EQ(my_seq->elements().at(0)->i64(), 1);
  EXPECT_EQ(my_seq->elements().at(1)->i64(), 3);
}

TEST_F(RemoveBangFunction, rejects_map_target)
{
  EXPECT_THROW(runtime.eval("(remove! keyword? {:a 1})"), Lisple::InvocationException);
}

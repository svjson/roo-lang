#include "runtime_fixture.h"
#include <gtest/gtest.h>

using FindFirstFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * FindFirstFunction - (find-first [...] predicate)
 * ======================================================================
 */

TEST_F(FindFirstFunction, find_first_vector)
{
  // Given
  runtime.eval(R"((def my-vector ["AA" "BB" "CCC" "DDDD" "EEE" "FF"]))");

  // When
  auto three_letter = runtime.eval("(find-first my-vector (fn [lmnt] (= 3 (count lmnt))))");
  auto four_letter = runtime.eval("(find-first my-vector (fn [lmnt] (= 4 (count lmnt))))");
  auto five_letter = runtime.eval("(find-first my-vector (fn [lmnt] (= 5 (count lmnt))))");

  // Then
  EXPECT_EQ(*three_letter, *Lisple::Value::string("CCC"));
  EXPECT_EQ(*four_letter, *Lisple::Value::string("DDDD"));
  EXPECT_EQ(*five_letter, *Lisple::Constant::NIL);
}

#include <lisple/runtime.h>

#include <gtest/gtest.h>

/*
 * ======================================================================
 * FindFirstFunction - (find-first [...] predicate)
 * ======================================================================
 */

TEST(FindFirstFunction, find_first_array)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"((def my-array ["AA" "BB" "CCC" "DDDD" "EEE" "FF"]))");

  // When
  auto three_letter = runtime.eval("(find-first my-array (fn [lmnt] (= 3 (count lmnt))))");
  auto four_letter = runtime.eval("(find-first my-array (fn [lmnt] (= 4 (count lmnt))))");
  auto five_letter = runtime.eval("(find-first my-array (fn [lmnt] (= 5 (count lmnt))))");

  // Then
  EXPECT_EQ(*three_letter, *Lisple::RTValue::string("CCC"));
  EXPECT_EQ(*four_letter, *Lisple::RTValue::string("DDDD"));
  EXPECT_EQ(*five_letter, *Lisple::Constant::NIL);
}

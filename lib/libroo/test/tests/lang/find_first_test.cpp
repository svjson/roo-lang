#include <vector>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/adapter.h>

using FindFirstFunction = RooTest::RuntimeTestFixture;
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
  EXPECT_EQ(*three_letter, *Roo::Value::string("CCC"));
  EXPECT_EQ(*four_letter, *Roo::Value::string("DDDD"));
  EXPECT_EQ(*five_letter, *Roo::Constant::NIL);
}

TEST_F(FindFirstFunction, finds_in_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((find-first "abc" (fn [c] (= c 'b'))))")->to_string(), "'b'");
}

TEST_F(FindFirstFunction, finds_in_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(find-first {:a 1 :b 2} keyword?)")->to_string(), ":a");
}

TEST_F(FindFirstFunction, finds_in_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(find-first values even?)")->to_string(), "2");
}

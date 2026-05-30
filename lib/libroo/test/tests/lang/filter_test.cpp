
#include <vector>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/adapter.h>
#include <roo/runtime/seq.h>

using FilterFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * FilterFunction - (filter [...] predicate)
 * ======================================================================
 */

TEST_F(FilterFunction, filter_vector)
{
  // Given
  auto result = runtime.eval("(filter [1 2 3 4 5 6] even?)");

  // Then
  ASSERT_TRUE(Roo::Type::VECTOR.is_type_of(*result));

  ASSERT_EQ(Roo::count(*result), 3);

  EXPECT_EQ(result->elements().at(0)->i64(), 2);
  EXPECT_EQ(result->elements().at(1)->i64(), 4);
  EXPECT_EQ(result->elements().at(2)->i64(), 6);
}

TEST_F(FilterFunction, filters_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((filter "abc" (fn [c] (= c 'b'))))")->to_string(), "['b']");
}

TEST_F(FilterFunction, filters_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(filter {:a 1 :b 2} keyword?)")->to_string(), "[:a :b]");
}

TEST_F(FilterFunction, filters_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(filter values even?)")->to_string(), "[2]");
}

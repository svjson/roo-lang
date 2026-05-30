
#include <vector>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/adapter.h>

using LastFunction = RooTest::RuntimeTestFixture;
/*
 * ===================================
 * LastFunction - (last [...])
 * ===================================
 */

TEST_F(LastFunction, last_vector)
{
  // Given
  EXPECT_EQ(runtime.eval("(last [8 9 10])")->to_string(), "10");
}

TEST_F(LastFunction, last_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((last "abc"))")->to_string(), "'c'");
}

TEST_F(LastFunction, last_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(last {:a 1 :b 2})")->to_string(), "2");
}

TEST_F(LastFunction, last_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(last values)")->to_string(), "3");
}

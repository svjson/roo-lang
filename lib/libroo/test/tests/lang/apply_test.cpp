
#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ApplyFunction = RooTest::RuntimeTestFixture;
TEST_F(ApplyFunction, apply_concat)
{
  // Given
  auto retval = runtime.eval("(apply concat [[1 2 3] [4 5 6]])");

  // Then
  ASSERT_EQ(retval->to_string(), "[1 2 3 4 5 6]");
}

TEST_F(ApplyFunction, apply_dynamic)
{
  // Given
  runtime.eval("(def function-name 'max)");
  auto result = runtime.eval("(apply (resolve function-name) [10 8 17 4 0])");

  // Then
  ASSERT_EQ(result->to_string(), "17");
}

TEST_F(ApplyFunction, applies_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((apply str "ab"))")->to_string(), R"("ab")");
}

TEST_F(ApplyFunction, applies_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(apply vector {:a 1})")->to_string(), "[:a 1]");
}

TEST_F(ApplyFunction, applies_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(apply + values)")->to_string(), "6");
}

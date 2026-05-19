#include <vector>

#include <lisple/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using KeepFunction = LispleTest::RuntimeTestFixture;
/*
 * ======================================================================
 * KeepFunction - (keep [...] select-transform-fm)
 * ======================================================================
 */

TEST_F(KeepFunction, transform_even)
{
  // Given
  auto result =
    runtime.eval("(keep [1 2 3 4] (fn [x] (when (even? x) (str \"Number \" x))))");

  // Then
  ASSERT_EQ(*result,
            *Lisple::Value::vector(
              {Lisple::Value::string("Number 2"), Lisple::Value::string("Number 4")}));
}

TEST_F(KeepFunction, keeps_from_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((keep "abc" (fn [c] (if (= c 'b') (str c) nil))))")->to_string(),
            R"(["b"])");
}

TEST_F(KeepFunction, keeps_from_map_as_interleaved_sequence)
{
  EXPECT_EQ(
    runtime.eval("(keep {:a 1 :b 2} (fn [x] (if (keyword? x) (name x) nil)))")->to_string(),
    R"(["a" "b"])");
}

TEST_F(KeepFunction, keeps_from_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(keep values (fn [n] (if (even? n) (* n 10) nil)))")->to_string(),
            "[20]");
}

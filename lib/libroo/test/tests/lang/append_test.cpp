#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AppendFunction = RooTest::RuntimeTestFixture;

TEST_F(AppendFunction, appends_element_to_vector)
{
  // Then
  EXPECT_EQ(runtime.eval("(append [1 2 3] 4)")->to_string(), "[1 2 3 4]");
}

TEST_F(AppendFunction, appends_to_empty_vector)
{
  // Then
  EXPECT_EQ(runtime.eval("(append [] :a)")->to_string(), "[:a]");
}

TEST_F(AppendFunction, appends_to_nil)
{
  // Then
  EXPECT_EQ(runtime.eval("(append nil 42)")->to_string(), "[42]");
}

TEST_F(AppendFunction, does_not_mutate_original)
{
  // Given
  runtime.eval("(def v [1 2 3])");

  // When
  runtime.eval("(append v 4)");

  // Then
  EXPECT_EQ(runtime.eval("v")->to_string(), "[1 2 3]");
}

TEST_F(AppendFunction, appends_any_value_type)
{
  // Then
  EXPECT_EQ(runtime.eval("(append [] \"hello\")")->to_string(), "[\"hello\"]");
  EXPECT_EQ(runtime.eval("(append [] {:a 1})")->to_string(), "[{:a 1}]");
  EXPECT_EQ(runtime.eval("(append [] [1 2])")->to_string(), "[[1 2]]");
}

TEST_F(AppendFunction, appends_multiple_elements)
{
  // Then
  EXPECT_EQ(runtime.eval("(append [1 2] 3 4 5)")->to_string(), "[1 2 3 4 5]");
}

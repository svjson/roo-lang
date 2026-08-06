
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using LetForm = RooTest::RuntimeTestFixture;
TEST_F(LetForm, define_and_exec_let)
{
  // When
  auto retval = runtime.eval("(let [x 10 y 20] (+ x y))");

  // Then
  ASSERT_EQ(retval->to_string(), "30");
  ASSERT_EQ(ctx.stack_size(), 1);
}

TEST_F(LetForm, define_and_exec_let_with_dynamic_value)
{
  // When
  auto retval = runtime.eval("(let [x (+ 20 20)] (+ x 10))");

  // Then
  ASSERT_EQ(retval->i64(), 50);
  ASSERT_EQ(ctx.stack_size(), 1);
}

TEST_F(LetForm, define_and_exec_let_with_dependent_bindings)
{
  // When
  auto retval = runtime.eval("(let [x 10 y (+ x 20)] (+ x y))");

  // Then
  ASSERT_EQ(retval->i64(), 40);
  ASSERT_EQ(ctx.stack_size(), 1);
}

TEST_F(LetForm, define_and_exec_let_with_dynamic_values)
{
  // When
  auto retval = runtime.eval("(let [x (+ 20 20) y (/ 90 2)] (+ x y))");

  // Then
  ASSERT_EQ(retval->i64(), 85);
  ASSERT_EQ(ctx.stack_size(), 1);
}

TEST_F(LetForm, destructure_vector)
{
  // Given
  auto result = runtime.eval("(let [[a b] [10 25]] (+ a b))");

  // Then
  ASSERT_EQ(result->to_string(), "35");
}

TEST_F(LetForm, destructure_vector_binds_missing_values_to_nil)
{
  // Given
  auto result = runtime.eval("(let [[a b] [:a]] b)");

  // Then
  ASSERT_EQ(result->to_string(), "nil");
}

TEST_F(LetForm, destructure_map)
{
  // Given
  auto result = runtime.eval("(let [{:keys [a b]} {:a 10 :b 25}] (+ a b))");

  // Then
  ASSERT_EQ(result->i64(), 35);
}

TEST_F(LetForm, bound_value_and_source_are_the_same)
{
  // Given
  runtime.switch_namespace("user");
  runtime.eval("(def state {:nested {:x 10 :y 8}})");

  // When
  auto result = runtime.eval("(let [nested (:nested state)] (assoc! nested :y 100))");

  // Then
  EXPECT_EQ(runtime.lookup("state")->to_string(), "{:nested {:x 10 :y 100}}");
}

TEST_F(LetForm, empty_let_body_returns_nil)
{
  // Given
  auto result = runtime.eval("(let [{:keys [a b]} {:a 10 :b 25}])");

  // Then
  ASSERT_TRUE(result.get());
  ASSERT_EQ(result, Roo::Constant::NIL);
}

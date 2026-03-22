
#include "../runtime_fixture.h"
#include <gtest/gtest.h>

TEST(LetForm, define_and_exec_let)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  Lisple::sptr_sobject retval = fixture.runtime.eval("(let [x 10 y 20] (+ x y))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(30));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetForm, define_and_exec_let_with_dynamic_value)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  Lisple::sptr_sobject retval = fixture.runtime.eval("(let [x (+ 20 20)] (+ x 10))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(50));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetForm, define_and_exec_let_with_dependent_bindings)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  Lisple::sptr_sobject retval = fixture.runtime.eval("(let [x 10 y (+ x 20)] (+ x y))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(40));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetForm, define_and_exec_let_with_dynamic_values)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  Lisple::sptr_sobject retval =
    fixture.runtime.eval("(let [x (+ 20 20) y (/ 90 2)] (+ x y))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(85));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetForm, destructure_array)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(let [[a b] [10 25]] (+ a b))");

  // Then
  ASSERT_EQ(*result, *Lisple::Number::make(35));
}

TEST(LetForm, destructure_map)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(let [{:keys [a b]} {:a 10 :b 25}] (+ a b))");

  // Then
  ASSERT_EQ(*result, *Lisple::Number::make(35));
}

TEST(LetForm, bound_value_and_source_are_the_same)
{
  // Given
  Lisple::Runtime runtime;
  runtime.switch_namespace("user");
  runtime.eval("(def state {:nested {:x 10 :y 8}})");

  // When
  Lisple::sptr_sobject result =
    runtime.eval("(let [nested (:nested state)] (assoc! nested :y 100))");

  // Then
  EXPECT_EQ(runtime.lookup("state")->to_string(), "{:nested {:x 10 :y 100}}");
}

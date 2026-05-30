
#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/exec.h>

using FnForm = RooTest::RuntimeTestFixture;
TEST_F(FnForm, define_and_exec_lambda)
{
  // Given
  Roo::sptr_ast_node_v code = parser.read_sexps("(fn [x y] (+ (+ x 1) (- y 2)))");

  Roo::sptr_val_v exec_args;
  exec_args.push_back(Roo::Value::number(5));
  exec_args.push_back(Roo::Value::number(5));

  // When
  auto fn = runtime.eval(code.front());
  auto retval = fn->exec().execute(ctx, exec_args);

  // Then
  EXPECT_TRUE(Roo::Type::NUMBER.is_type_of(*retval));
  EXPECT_EQ(retval->i64(), 9);
}

TEST_F(FnForm, with_bound_scope)
{
  // Given
  runtime.eval("(defun bind-for-add [bound-num] (fn [other] (+ bound-num other)))");
  runtime.eval("(def four-adder (bind-for-add 4))");

  // When
  auto result = runtime.eval("(apply four-adder [3])");

  // Then
  EXPECT_EQ(*result, *Roo::Value::number(7));
}

TEST_F(FnForm, optional_arg_bound_to_value_when_supplied)
{
  // Given
  runtime.eval("(def greet (fn [name & title] (str title \" \" name)))");

  // When
  auto result = runtime.eval("(greet \"Smith\" \"Dr.\")");

  // Then
  ASSERT_EQ(result->str(), "Dr. Smith");
}

TEST_F(FnForm, optional_arg_bound_to_nil_when_omitted)
{
  // Given
  runtime.eval("(def greet (fn [name & title] (if title (str title \" \" name) name)))");

  // When
  auto result = runtime.eval("(greet \"Smith\")");

  // Then
  ASSERT_EQ(result->str(), "Smith");
}

TEST_F(FnForm, multiple_optional_args_all_supplied)
{
  // Given
  auto result = runtime.eval("((fn [a & b c] [a b c]) 1 2 3)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 3]"));
}

TEST_F(FnForm, multiple_optional_args_partially_supplied)
{
  // Given
  auto result = runtime.eval("((fn [a & b c] [a b c]) 1 2)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 nil]"));
}

TEST_F(FnForm, multiple_optional_args_none_supplied)
{
  // Given
  auto result = runtime.eval("((fn [a & b c] [a b c]) 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 nil nil]"));
}

TEST_F(FnForm, too_many_args_with_optional_but_no_rest_throws)
{
  // Given
  EXPECT_THROW(runtime.eval("((fn [a & b] b) 1 2 3)"), std::exception);
}

TEST_F(FnForm, rest_parameter_collects_all_extra_args)
{
  // Given
  auto result = runtime.eval("((fn [a &rest] [a rest]) 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 [2 3 4]]"));
}

TEST_F(FnForm, rest_parameter_is_empty_vector_when_no_extra_args)
{
  // Given
  auto result = runtime.eval("((fn [a &rest] [a rest]) 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 []]"));
}

TEST_F(FnForm, rest_parameter_after_optional_args)
{
  // Given
  auto result = runtime.eval("((fn [a & b &rest] [a b rest]) 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 [3 4]]"));
}

TEST_F(FnForm, signals_correct_exec_path)
{
  // Given
  auto result = runtime.eval("(fn [n] (+ n 10))");
  Roo::Executable& fn = result->exec();

  // When
  EXPECT_FALSE(fn.supports_exec_tree());
  EXPECT_EQ(fn.get_signatures().size(), 1);
  EXPECT_FALSE(fn.get_signatures().front()->supports_exec_tree());
  EXPECT_TRUE(fn.get_signatures().front()->supports_rt_value());
}

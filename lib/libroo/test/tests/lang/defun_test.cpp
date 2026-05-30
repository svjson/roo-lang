
#include <roo/lang/func.h>

#include "../runtime_fixture.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DefunForm = RooTest::RuntimeTestFixture;
TEST_F(DefunForm, define_no_arg_fun)
{
  // Given
  Roo::sptr_val result = runtime.eval("(defun my-fn [] 8)");

  // Then
  auto fun = runtime.lookup(*Roo::Value::symbol("my-fn"));
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->type, Roo::Value::Type::FUNCTION);
  auto& user_fun = dynamic_cast<Roo::UserFunction&>(fun->exec());
  EXPECT_TRUE(user_fun.get_argument_bindings().empty());
  EXPECT_EQ(user_fun.get_body().size(), 1);
  EXPECT_EQ(*user_fun.get_body().front()->form, *Roo::AST::Number::make(8));
}

TEST_F(DefunForm, define_no_arg_fun_with_docstring)
{
  // Given
  Roo::sptr_val result = runtime.eval(R"(
    (defun my-fn
     "This function does all the magic things you can think of..."
     []
     8)
                                            )");

  // Then
  auto fun = runtime.lookup(*Roo::Value::symbol("my-fn"));
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->type, Roo::Value::Type::FUNCTION);
  auto& user_fun = dynamic_cast<Roo::UserFunction&>(fun->exec());
  EXPECT_TRUE(user_fun.get_argument_bindings().empty());
  EXPECT_EQ(user_fun.get_body().size(), 1);
  EXPECT_EQ(*user_fun.get_body().front()->form, *Roo::AST::Number::make(8));
}

TEST_F(DefunForm, defun_with_static_return_value)
{
  // Given
  auto result = runtime.eval("(defun gimme-five [] 5)");

  // Then
  ASSERT_TRUE(runtime.get_current_namespace().has("gimme-five"));
  Roo::sptr_val_v args;
  auto retval = result->exec().execute(ctx, args);
  ASSERT_EQ(retval->i64(), 5);
}

TEST_F(DefunForm, defun_with_single_argument)
{
  // Given
  auto result = runtime.eval("(defun add-five [x] (+ x 5))");

  // Then
  ASSERT_TRUE(runtime.get_current_namespace().has("add-five"));
  Roo::sptr_val_v args{Roo::Value::number(6)};
  auto retval = result->exec().execute(ctx, args);
  ASSERT_EQ(retval->i64(), 11);
}

TEST_F(DefunForm, defun_with_destructuring_argument)
{
  // Given
  runtime.eval("(defun myfun [{:keys [one two]}] [one two])");

  // When
  auto result = runtime.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2]"));
}

TEST_F(DefunForm, defun_with_destructuring_argument_and_alias)
{
  // Given
  runtime.eval("(defun myfun [{:keys [one two] :as seq}] [one two seq])");

  // When
  auto result = runtime.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 {:one 1 :two 2}]"));
}

TEST_F(DefunForm, optional_arg_bound_to_value_when_supplied)
{
  // Given
  runtime.eval("(defun greet [name & title] (str title \" \" name))");

  // When
  auto result = runtime.eval("(greet \"Smith\" \"Dr.\")");

  // Then
  ASSERT_EQ(result->str(), "Dr. Smith");
}

TEST_F(DefunForm, optional_arg_bound_to_nil_when_omitted)
{
  // Given
  runtime.eval("(defun greet [name & title] (if title (str title \" \" name) name))");

  // When
  auto result = runtime.eval("(greet \"Smith\")");

  // Then
  ASSERT_EQ(result->str(), "Smith");
}

TEST_F(DefunForm, multiple_optional_args_all_supplied)
{
  // Given
  runtime.eval("(defun f [a & b c] [a b c])");

  // When
  auto result = runtime.eval("(f 1 2 3)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 3]"));
}

TEST_F(DefunForm, multiple_optional_args_partially_supplied)
{
  // Given
  runtime.eval("(defun f [a & b c] [a b c])");

  // When
  auto result = runtime.eval("(f 1 2)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 nil]"));
}

TEST_F(DefunForm, multiple_optional_args_none_supplied)
{
  // Given
  runtime.eval("(defun f [a & b c] [a b c])");

  // When
  auto result = runtime.eval("(f 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 nil nil]"));
}

TEST_F(DefunForm, too_many_args_with_optional_but_no_rest_throws)
{
  // Given
  runtime.eval("(defun f [a & b] b)");

  // When / Then
  EXPECT_THROW(runtime.eval("(f 1 2 3)"), std::exception);
}

TEST_F(DefunForm, rest_parameter_collects_all_extra_args)
{
  // Given
  runtime.eval("(defun f [a &rest] [a rest])");

  // When
  auto result = runtime.eval("(f 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 [2 3 4]]"));
}

TEST_F(DefunForm, rest_parameter_is_empty_vector_when_no_extra_args)
{
  // Given
  runtime.eval("(defun f [a &rest] [a rest])");

  // When
  auto result = runtime.eval("(f 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 []]"));
}

TEST_F(DefunForm, rest_parameter_after_optional_args)
{
  // Given
  runtime.eval("(defun f [a & b &rest] [a b rest])");

  // When
  auto result = runtime.eval("(f 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 [3 4]]"));
}

TEST_F(DefunForm, execution_treats_explicit_nil_argument_with_value_as_nil)
{
  // Given
  runtime.eval("(defun myfun [input] (if input (str \"Input is\" input) \"nil input\"))");

  // When
  auto result = runtime.eval("(myfun nil)");

  // Then
  ASSERT_EQ(result->str(), "nil input");
}

TEST_F(DefunForm, execution_treats_derived_argument_with_nil_value_as_nil)
{
  // Given
  runtime.eval("(defun myfun [input] (if input (str \"Input is\" input) \"nil input\"))");

  // When
  auto result = runtime.eval("(myfun (:value {:no-value 1}))");

  // Then
  ASSERT_EQ(result->str(), "nil input");
}

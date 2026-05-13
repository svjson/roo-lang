
#include <lisple/lang/func.h>
#include <lisple/runtime.h>

#include "../runtime_fixture.h"
#include <gtest/gtest.h>

TEST(DefunForm, define_no_arg_fun)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_rtval result = runtime.eval("(defun my-fn [] 8)");

  // Then
  Lisple::Word fn_name("my-fn");
  auto fun = runtime.lookup_value(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->type, Lisple::RTValue::Type::FUNCTION);
  auto& user_fun = dynamic_cast<Lisple::UserFunction&>(fun->exec());
  EXPECT_TRUE(user_fun.get_argument_bindings().empty());
  EXPECT_EQ(user_fun.get_body().size(), 1);
  EXPECT_EQ(*user_fun.get_body().front()->form,
            *Lisple::Number::make(8));
}

TEST(DefunForm, define_no_arg_fun_with_docstring)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_rtval result = runtime.eval(R"(
    (defun my-fn
     "This function does all the magic things you can think of..."
     []
     8)
                                            )");

  // Then
  Lisple::Word fn_name("my-fn");
  auto fun = runtime.lookup_value(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->type, Lisple::RTValue::Type::FUNCTION);
  auto& user_fun = dynamic_cast<Lisple::UserFunction&>(fun->exec());
  EXPECT_TRUE(user_fun.get_argument_bindings().empty());
  EXPECT_EQ(user_fun.get_body().size(), 1);
  EXPECT_EQ(*user_fun.get_body().front()->form,
            *Lisple::Number::make(8));
}

TEST(DefunForm, defun_with_static_return_value)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(defun gimme-five [] 5)");

  // Then
  ASSERT_TRUE(runtime.get_current_namespace().has(Lisple::Word("gimme-five")));
  Lisple::sptr_rtval_v args;
  Lisple::Context ctx(runtime);
  auto retval = result->exec().execute(ctx, args);
  ASSERT_EQ(retval->i64(), 5);
}

TEST(DefunForm, defun_with_single_argument)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(defun add-five [x] (+ x 5))");

  // Then
  ASSERT_TRUE(runtime.get_current_namespace().has(Lisple::Word("add-five")));
  Lisple::sptr_rtval_v args{Lisple::RTValue::number(6)};
  Lisple::Context ctx(runtime);
  auto retval = result->exec().execute(ctx, args);
  ASSERT_EQ(retval->i64(), 11);
}

TEST(DefunForm, defun_with_destructuring_argument)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun myfun [{:keys [one two]}] [one two])");

  // When
  auto result = runtime.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2]"));
}

TEST(DefunForm, defun_with_destructuring_argument_and_alias)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun myfun [{:keys [one two] :as seq}] [one two seq])");

  // When
  auto result = runtime.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 {:one 1 :two 2}]"));
}

TEST(DefunForm, optional_arg_bound_to_value_when_supplied)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun greet [name & title] (str title \" \" name))");

  // When
  auto result = runtime.eval("(greet \"Smith\" \"Dr.\")");

  // Then
  ASSERT_EQ(result->str(), "Dr. Smith");
}

TEST(DefunForm, optional_arg_bound_to_nil_when_omitted)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun greet [name & title] (if title (str title \" \" name) name))");

  // When
  auto result = runtime.eval("(greet \"Smith\")");

  // Then
  ASSERT_EQ(result->str(), "Smith");
}

TEST(DefunForm, multiple_optional_args_all_supplied)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun f [a & b c] [a b c])");

  // When
  auto result = runtime.eval("(f 1 2 3)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 3]"));
}

TEST(DefunForm, multiple_optional_args_partially_supplied)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun f [a & b c] [a b c])");

  // When
  auto result = runtime.eval("(f 1 2)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 nil]"));
}

TEST(DefunForm, multiple_optional_args_none_supplied)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun f [a & b c] [a b c])");

  // When
  auto result = runtime.eval("(f 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 nil nil]"));
}

TEST(DefunForm, too_many_args_with_optional_but_no_rest_throws)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun f [a & b] b)");

  // When / Then
  EXPECT_THROW(runtime.eval("(f 1 2 3)"), std::exception);
}

TEST(DefunForm, rest_parameter_collects_all_extra_args)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun f [a &rest] [a rest])");

  // When
  auto result = runtime.eval("(f 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 [2 3 4]]"));
}

TEST(DefunForm, rest_parameter_is_empty_vector_when_no_extra_args)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun f [a &rest] [a rest])");

  // When
  auto result = runtime.eval("(f 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 []]"));
}

TEST(DefunForm, rest_parameter_after_optional_args)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun f [a & b &rest] [a b rest])");

  // When
  auto result = runtime.eval("(f 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 [3 4]]"));
}

TEST(DefunForm, execution_treats_explicit_nil_argument_with_value_as_nil)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun myfun [input] (if input (str \"Input is\" input) \"nil input\"))");

  // When
  auto result = runtime.eval("(myfun nil)");

  // Then
  ASSERT_EQ(result->str(), "nil input");
}

TEST(DefunForm, execution_treats_derived_argument_with_nil_value_as_nil)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun myfun [input] (if input (str \"Input is\" input) \"nil input\"))");

  // When
  auto result = runtime.eval("(myfun (:value {:no-value 1}))");

  // Then
  ASSERT_EQ(result->str(), "nil input");
}

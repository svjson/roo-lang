
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
  auto fun = runtime.lookup(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->get_type(), Lisple::Form::FUNCTION);
  EXPECT_TRUE(fun->as<Lisple::UserFunction>().get_argument_bindings().empty());
  EXPECT_EQ(fun->as<Lisple::UserFunction>().get_body().size(), 1);
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front()->form,
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
  auto fun = runtime.lookup(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->get_type(), Lisple::Form::FUNCTION);
  EXPECT_TRUE(fun->as<Lisple::UserFunction>().get_argument_bindings().empty());
  EXPECT_EQ(fun->as<Lisple::UserFunction>().get_body().size(), 1);
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front()->form,
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


#include <lisple/exec.h>

#include "../runtime_fixture.h"
#include <gtest/gtest.h>

TEST(FnForm, define_and_exec_lambda)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(fn [x y] (+ (+ x 1) (- y 2)))");

  Lisple::sptr_sobject_v exec_args;
  exec_args.push_back(std::make_shared<Lisple::Number>(5));
  exec_args.push_back(std::make_shared<Lisple::Number>(5));

  // When
  auto fn = fixture.runtime.eval(code.front());
  auto retval = fn->exec().execute(fixture.ctx, exec_args);

  // Then
  EXPECT_TRUE(Lisple::Type::NUMBER.is_type_of(*retval));
  EXPECT_EQ(retval->as<Lisple::Number>().value, 9);
}

TEST(FnForm, with_bound_scope)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(defun bind-for-add [bound-num] (fn [other] (+ bound-num other)))");
  fixture.runtime.eval("(def four-adder (bind-for-add 4))");

  // When
  auto result = fixture.runtime.eval("(apply four-adder [3])");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(7));
}

TEST(FnForm, optional_arg_bound_to_value_when_supplied)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def greet (fn [name & title] (str title \" \" name)))");

  // When
  auto result = runtime.eval("(greet \"Smith\" \"Dr.\")");

  // Then
  ASSERT_EQ(result->str(), "Dr. Smith");
}

TEST(FnForm, optional_arg_bound_to_nil_when_omitted)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def greet (fn [name & title] (if title (str title \" \" name) name)))");

  // When
  auto result = runtime.eval("(greet \"Smith\")");

  // Then
  ASSERT_EQ(result->str(), "Smith");
}

TEST(FnForm, multiple_optional_args_all_supplied)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("((fn [a & b c] [a b c]) 1 2 3)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 3]"));
}

TEST(FnForm, multiple_optional_args_partially_supplied)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("((fn [a & b c] [a b c]) 1 2)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 nil]"));
}

TEST(FnForm, multiple_optional_args_none_supplied)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("((fn [a & b c] [a b c]) 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 nil nil]"));
}

TEST(FnForm, too_many_args_with_optional_but_no_rest_throws)
{
  // Given
  Lisple::Runtime runtime;

  // When / Then
  EXPECT_THROW(runtime.eval("((fn [a & b] b) 1 2 3)"), std::exception);
}

TEST(FnForm, rest_parameter_collects_all_extra_args)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("((fn [a &rest] [a rest]) 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 [2 3 4]]"));
}

TEST(FnForm, rest_parameter_is_empty_vector_when_no_extra_args)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("((fn [a &rest] [a rest]) 1)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 []]"));
}

TEST(FnForm, rest_parameter_after_optional_args)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("((fn [a & b &rest] [a b rest]) 1 2 3 4)");

  // Then
  ASSERT_EQ(*result, *runtime.eval("[1 2 [3 4]]"));
}

TEST(FnForm, signals_correct_exec_path)
{
  // Given
  Lisple::Runtime runtime;
  auto result = runtime.eval("(fn [n] (+ n 10))");
  Lisple::Executable& fn = result->exec();

  // When
  EXPECT_FALSE(fn.supports_exec_tree());
  EXPECT_EQ(fn.get_signatures().size(), 1);
  EXPECT_FALSE(fn.get_signatures().front()->supports_exec_tree());
  EXPECT_TRUE(fn.get_signatures().front()->supports_rt_value());
}

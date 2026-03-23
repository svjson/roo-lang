
#include "../runtime_fixture.h"
#include <gtest/gtest.h>
#include <lisple/lang/func.h>
#include <lisple/runtime.h>

TEST(DefunMacro, define_no_arg_fun)
{
  LispleTest::RuntimeFixture fixture;

  Lisple::sptr_sobject_v args{std::make_shared<Lisple::Word>("my-fn"),
                              std::make_shared<Lisple::Array>(),
                              std::make_shared<Lisple::Number>(8)};

  Lisple::DefunForm defun;

  // When
  defun.execute(fixture.ctx, args);

  // Then
  Lisple::Word fn_name("my-fn");
  auto fun = fixture.ctx.lookup(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->get_type(), Lisple::Form::FUNCTION);
  EXPECT_TRUE(fun->as<Lisple::UserFunction>().get_argument_bindings().empty());
  EXPECT_EQ(fun->as<Lisple::UserFunction>().get_body().size(), 1);
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front()->form,
            *Lisple::Number::make(8));
}

TEST(DefunMacro, define_no_arg_fun_with_docstring)
{
  LispleTest::RuntimeFixture fixture;

  Lisple::sptr_sobject_v args{
    std::make_shared<Lisple::Word>("my-fn"),
    std::make_shared<Lisple::String>(
      "This function does all the magic things you can think of..."),
    std::make_shared<Lisple::Array>(),
    std::make_shared<Lisple::Number>(8)};

  Lisple::DefunForm defun;

  // When
  defun.execute(fixture.ctx, args);

  // Then
  Lisple::Word fn_name("my-fn");
  auto fun = fixture.ctx.lookup(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->get_type(), Lisple::Form::FUNCTION);
  EXPECT_TRUE(fun->as<Lisple::UserFunction>().get_argument_bindings().empty());
  EXPECT_EQ(fun->as<Lisple::UserFunction>().get_body().size(), 1);
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front()->form,
            *Lisple::Number::make(8));
}


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
  auto retval = fn->execute(fixture.ctx, exec_args);

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
  Lisple::sptr_sobject result = fixture.runtime.eval("(apply four-adder [3])");

  // Then
  EXPECT_EQ(*result, Lisple::Number(7));
}

TEST(FnForm, signals_correct_exec_path)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::sptr_sobject result = runtime.eval("(fn [n] (+ n 10))");
  Lisple::Executable& fn = result->as<Lisple::Executable>();

  // When
  EXPECT_FALSE(fn.supports_exec_tree());
  EXPECT_EQ(fn.get_signatures().size(), 1);
  EXPECT_FALSE(fn.get_signatures().front()->supports_exec_tree());
  EXPECT_TRUE(fn.get_signatures().front()->supports_rt_value());
}

#include "runtime_fixture.h"
#include <execinfo.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <lisple/form.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>
#include <lisple/type.h>
#include <memory>

TEST(DefunTest, defun_with_static_return_value)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(defun gimme-five [] 5)");

  // When
  auto result = fixture.runtime.eval(code.at(0));

  // Then
  ASSERT_TRUE(fixture.runtime.get_current_namespace().has(Lisple::Word("gimme-five")));
  Lisple::sptr_sobject_v args;
  auto retval = result->execute(fixture.ctx, args);
  ASSERT_EQ(retval->as<Lisple::Number>().int_value(), 5);
}

TEST(DefunTest, defun_with_single_argument)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(defun add-five [x] (+ x 5))");

  // When
  auto result = fixture.runtime.eval(code.at(0));

  // Then
  ASSERT_TRUE(fixture.runtime.get_current_namespace().has(Lisple::Word("add-five")));
  Lisple::sptr_sobject_v args{std::make_shared<Lisple::Number>(6)};
  auto retval = result->execute(fixture.ctx, args);
  ASSERT_EQ(retval->as<Lisple::Number>().int_value(), 11);
}

TEST(DefunTest, defun_with_destructuring_argument)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(defun myfun [{:keys [one two]}] [one two])");

  // When
  auto result = fixture.runtime.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *fixture.runtime.eval("[1 2]"));
}

TEST(DefunTest, defun_with_destructuring_argument_and_alias)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(defun myfun [{:keys [one two] :as seq}] [one two seq])");

  // When
  auto result = fixture.runtime.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *fixture.runtime.eval("[1 2 {:one 1 :two 2}]"));
}

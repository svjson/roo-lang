
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <memory>

#include <lisple/lisp_reader.h>
#include <lisple/namespace.h>

#include <lisple/form.h>
#include <lisple/reader.h>
#include <lisple/type.h>

#include "lisp_reader_fixture.h"

TEST(DefunTest, defun_with_static_return_value)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(defun gimme-five [] 5)");

  // When
  auto result = fixture.lisp_reader.eval(code.at(0));

  // Then
  ASSERT_TRUE(fixture.lisp_reader.get_current_namespace().has(Lisple::Word("gimme-five")));
  Lisple::sptr_sobject_v args;
  auto retval = result->execute(fixture.ctx, args);
  ASSERT_EQ(retval->as<Lisple::Number>().int_value(), 5);
}

TEST(DefunTest, defun_with_single_argument)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(defun add-five [x] (+ x 5))");

  // When
  auto result = fixture.lisp_reader.eval(code.at(0));

  // Then
  ASSERT_TRUE(fixture.lisp_reader.get_current_namespace().has(Lisple::Word("add-five")));
  Lisple::sptr_sobject_v args
  {
    std::make_shared<Lisple::Number>(6)
  };
  auto retval = result->execute(fixture.ctx, args);
  ASSERT_EQ(retval->as<Lisple::Number>().int_value(), 11);
}

TEST(DefunTest, defun_with_destructuring_argument)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(defun myfun [{:keys [one two]}] [one two])");

  // When
  auto result = fixture.lisp_reader.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *fixture.lisp_reader.eval("[1 2]"));
}

TEST(DefunTest, defun_with_destructuring_argument_and_alias)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(defun myfun [{:keys [one two] :as seq}] [one two seq])");

  // When
  auto result = fixture.lisp_reader.eval("(myfun {:one 1 :two 2})");

  // Then
  ASSERT_EQ(*result, *fixture.lisp_reader.eval("[1 2 {:one 1 :two 2}]"));
}

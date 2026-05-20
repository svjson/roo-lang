#include "runtime_fixture.h"

#include <gtest/gtest.h>
#include <lisple/exception.h>

using OperatorTypeGuard = LispleTest::RuntimeTestFixture;

TEST_F(OperatorTypeGuard, arithmetic_operators_throw_type_error_for_nil_args)
{
  EXPECT_THROW(runtime.eval("(+ nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(+ 1 nil)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(- nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(- 1 nil)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(/ nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(/ 1 nil)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(* nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(* 1 nil)"), Lisple::TypeError);
}

TEST_F(OperatorTypeGuard, bit_shift_operators_throw_type_error_for_nil_args)
{
  EXPECT_THROW(runtime.eval("(<< nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(<< 1 nil)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(>> nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(>> 1 nil)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(<<< nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(<<< 1 nil)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(>>> nil 1)"), Lisple::TypeError);
  EXPECT_THROW(runtime.eval("(>>> 1 nil)"), Lisple::TypeError);
}

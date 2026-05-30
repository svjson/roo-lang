#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using OperatorTypeGuard = RooTest::RuntimeTestFixture;

TEST_F(OperatorTypeGuard, arithmetic_operators_throw_type_error_for_nil_args)
{
  EXPECT_THROW(runtime.eval("(+ nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(+ 1 nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(- nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(- 1 nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(/ nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(/ 1 nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(* nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(* 1 nil)"), Roo::TypeError);
}

TEST_F(OperatorTypeGuard, bit_shift_operators_throw_type_error_for_nil_args)
{
  EXPECT_THROW(runtime.eval("(<< nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(<< 1 nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(>> nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(>> 1 nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(<<< nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(<<< 1 nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(>>> nil 1)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(>>> 1 nil)"), Roo::TypeError);
}

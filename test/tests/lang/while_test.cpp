#include "runtime_fixture.h"

#include <gtest/gtest.h>


using WhileMacro = LispleTest::RuntimeTestFixture;
TEST_F(WhileMacro, loop_with_counter)
{
  // Given
  runtime.eval("(def x 0)");

  // When
  auto retval = runtime.eval("(while (not= x 10) (set! [x] (+ x 1)))");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.get_current_namespace().lookup("x"),
            *Lisple::RTValue::number(10));
}

TEST_F(WhileMacro, multi_form_loop)
{
  // Given
  runtime.eval("(def x 0)");

  // When
  auto retval = runtime.eval("(while (not= x 10) (set! [x] (+ x 1)) (* x 2))");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(20));
  EXPECT_EQ(*runtime.get_current_namespace().lookup("x"),
            *Lisple::RTValue::number(10));
}

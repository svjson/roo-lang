#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DoForm = RooTest::RuntimeTestFixture;

TEST_F(DoForm, no_body_returns_nil)
{
  auto result = runtime.eval("(do)");

  ASSERT_TRUE(result.get());
  ASSERT_EQ(result, Roo::Constant::NIL);
}


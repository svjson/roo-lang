#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IdentityFunction = RooTest::RuntimeTestFixture;

TEST_F(IdentityFunction, returns_its_argument_unchanged)
{
  // Then
  EXPECT_EQ(runtime.eval("(identity 42)")->to_string(), "42");
  EXPECT_EQ(runtime.eval("(identity \"hello\")")->to_string(), "\"hello\"");
  EXPECT_EQ(runtime.eval("(identity [:a :b])")->to_string(), "[:a :b]");
}

TEST_F(IdentityFunction, returns_nil_unchanged)
{
  // Then
  EXPECT_EQ(runtime.eval("(identity nil)")->to_string(), "nil");
}

TEST_F(IdentityFunction, usable_as_higher_order_argument)
{
  // Then
  EXPECT_EQ(runtime.eval("(filter [1 nil 2 nil 3] identity)")->to_string(), "[1 2 3]");
}

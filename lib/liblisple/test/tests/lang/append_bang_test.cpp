#include <lisple/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AppendBangFunction = LispleTest::RuntimeTestFixture;

TEST_F(AppendBangFunction, appends_to_vector)
{
  runtime.eval("(def values [1 2])");

  EXPECT_EQ(runtime.eval("(append! values 3)")->to_string(), "[1 2 3]");
  EXPECT_EQ(runtime.lookup("values")->to_string(), "[1 2 3]");
}

TEST_F(AppendBangFunction, rejects_map_target)
{
  EXPECT_THROW(runtime.eval("(append! {:a 1} 2)"), Lisple::InvocationException);
}

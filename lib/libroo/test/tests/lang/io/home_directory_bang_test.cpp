#include "runtime_fixture.h"
#include <gtest/gtest.h>

using HomeDirectoryBang = RooTest::RuntimeTestFixture;

TEST_F(HomeDirectoryBang, returns_runtime_home_directory)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/home-directory!))")->to_string(), R"("/home/fake")");
}

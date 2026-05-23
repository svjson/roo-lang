#include "runtime_fixture.h"

#include <gtest/gtest.h>

using HomeDirectoryBang = LispleTest::RuntimeTestFixture;

TEST_F(HomeDirectoryBang, returns_runtime_home_directory)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/home-directory!))")->to_string(),
            R"("/home/fake")");
}

#include "runtime_fixture.h"

#include <gtest/gtest.h>

using CurrentDirectoryBang = LispleTest::RuntimeTestFixture;

TEST_F(CurrentDirectoryBang, returns_runtime_current_directory)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/current-directory!))")->to_string(), R"("/fake")");
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CurrentDirectoryBang = RooTest::RuntimeTestFixture;

TEST_F(CurrentDirectoryBang, returns_runtime_current_directory)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/current-directory!))")->to_string(), R"("/fake")");
}

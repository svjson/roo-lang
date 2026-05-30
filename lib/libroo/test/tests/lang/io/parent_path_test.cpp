#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ParentPath = RooTest::RuntimeTestFixture;

TEST_F(ParentPath, returns_parent_path)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/parent-path "assets/config.edn"))")->to_string(),
            R"("assets")");
}

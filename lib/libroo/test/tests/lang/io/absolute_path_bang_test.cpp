#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AbsolutePathBang = RooTest::RuntimeTestFixture;

TEST_F(AbsolutePathBang, returns_runtime_absolute_path)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/absolute-path! "assets/config.edn"))")->to_string(),
            R"("/fake/assets/config.edn")");
}

#include "runtime_fixture.h"

#include <gtest/gtest.h>

using Basename = LispleTest::RuntimeTestFixture;

TEST_F(Basename, returns_last_path_component)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/basename "assets/config.edn"))")->to_string(),
            R"("config.edn")");
}

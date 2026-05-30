#include "runtime_fixture.h"
#include <gtest/gtest.h>

using JoinPath = RooTest::RuntimeTestFixture;

TEST_F(JoinPath, joins_path_components)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/join-path "assets" "config.edn"))")->to_string(),
            R"("assets/config.edn")");
}

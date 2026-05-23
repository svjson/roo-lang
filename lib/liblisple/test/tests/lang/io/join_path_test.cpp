#include "runtime_fixture.h"

#include <gtest/gtest.h>

using JoinPath = LispleTest::RuntimeTestFixture;

TEST_F(JoinPath, joins_path_components)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/join-path "assets" "config.edn"))")->to_string(),
            R"("assets/config.edn")");
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Dirname = RooTest::RuntimeTestFixture;

TEST_F(Dirname, returns_parent_directory_component)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/dirname "assets/config.edn"))")->to_string(),
            R"("assets")");
}

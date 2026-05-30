#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ExistsP = RooTest::RuntimeTestFixture;

TEST_F(ExistsP, observes_existing_and_missing_paths)
{
  fs.add_file("assets/config.edn", "{}");

  EXPECT_EQ(runtime.eval(R"((roo.io/exists? "assets/config.edn"))")->to_string(), "true");
  EXPECT_EQ(runtime.eval(R"((roo.io/exists? "assets/missing.edn"))")->to_string(), "false");
}

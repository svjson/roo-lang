#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DirectoryP = RooTest::RuntimeTestFixture;

TEST_F(DirectoryP, observes_directories)
{
  fs.add_file("assets/config.edn", "{}");

  EXPECT_EQ(runtime.eval(R"((roo.io/directory? "assets"))")->to_string(), "true");
  EXPECT_EQ(runtime.eval(R"((roo.io/directory? "assets/config.edn"))")->to_string(),
            "false");
}

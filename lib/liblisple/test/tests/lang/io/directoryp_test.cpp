#include "runtime_fixture.h"

#include <gtest/gtest.h>

using DirectoryP = LispleTest::RuntimeTestFixture;

TEST_F(DirectoryP, observes_directories)
{
  fs.add_file("assets/config.edn", "{}");

  EXPECT_EQ(runtime.eval(R"((lisple.io/directory? "assets"))")->to_string(), "true");
  EXPECT_EQ(runtime.eval(R"((lisple.io/directory? "assets/config.edn"))")->to_string(),
            "false");
}

#include "runtime_fixture.h"

#include <gtest/gtest.h>

using FileP = LispleTest::RuntimeTestFixture;

TEST_F(FileP, observes_files)
{
  fs.add_file("assets/config.edn", "{}");

  EXPECT_EQ(runtime.eval(R"((lisple.io/file? "assets/config.edn"))")->to_string(), "true");
  EXPECT_EQ(runtime.eval(R"((lisple.io/file? "assets"))")->to_string(), "false");
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Extension = RooTest::RuntimeTestFixture;

TEST_F(Extension, returns_path_extension)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/extension "assets/config.edn"))")->to_string(),
            R"(".edn")");
}

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SpitEdnBang = RooTest::RuntimeTestFixture;

TEST_F(SpitEdnBang, writes_value_representation)
{
  runtime.eval(R"((roo.io/spit-edn! "data.edn" {:a 1 :b [true nil]}))");

  ASSERT_TRUE(fs.has_file("data.edn"));
  EXPECT_EQ(fs.get_file("data.edn"), "{:a 1 :b [true nil]}");
}

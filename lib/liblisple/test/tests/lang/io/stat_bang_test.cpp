#include "runtime_fixture.h"

#include <gtest/gtest.h>

using StatBang = LispleTest::RuntimeTestFixture;

TEST_F(StatBang, returns_missing_and_file_records)
{
  fs.add_file("assets/config.edn", "12345");

  EXPECT_EQ(runtime.eval(R"((lisple.io/stat! "missing.edn"))")->to_string(),
            "{:exists? false :type nil :size nil :modified-ms nil}");
  EXPECT_EQ(runtime.eval(R"((lisple.io/stat! "assets/config.edn"))")->to_string(),
            "{:exists? true :type :file :size 5 :modified-ms 0}");
}

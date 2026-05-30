#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SpitBang = RooTest::RuntimeTestFixture;

TEST_F(SpitBang, writes_file_contents)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/spit! "notes.txt" "plain text"))")->to_string(), "nil");

  ASSERT_TRUE(fs.has_file("notes.txt"));
  EXPECT_EQ(fs.get_file("notes.txt"), "plain text");
}

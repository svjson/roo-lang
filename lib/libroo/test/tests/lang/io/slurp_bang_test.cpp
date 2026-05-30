#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SlurpBang = RooTest::RuntimeTestFixture;

TEST_F(SlurpBang, reads_file_contents)
{
  fs.add_file("notes.txt", "plain text\n");

  EXPECT_EQ(runtime.eval(R"((roo.io/slurp! "notes.txt"))")->to_string(),
            R"("plain text
")");
}

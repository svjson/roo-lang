#include "runtime_fixture.h"
#include <gtest/gtest.h>

using CopyFileBang = RooTest::RuntimeTestFixture;

TEST_F(CopyFileBang, copies_file_contents)
{
  fs.add_file("src/native/libexample.so", "binary-ish contents");

  EXPECT_EQ(
    runtime
      .eval(R"((roo.io/copy-file! "src/native/libexample.so" "repo/native/libexample.so"))")
      ->to_string(),
    "nil");

  ASSERT_TRUE(fs.has_file("repo/native/libexample.so"));
  EXPECT_EQ(fs.get_file("repo/native/libexample.so"), "binary-ish contents");
}

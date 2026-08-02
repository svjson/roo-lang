#include "runtime_fixture.h"
#include <gtest/gtest.h>

using AstReadString = RooTest::RuntimeTestFixture;

TEST_F(AstReadString, reads_source_string_with_default_source_name)
{
  EXPECT_EQ(
    runtime.eval(R"roo((:path (:source (nth (roo.ast/read-string "(def x 1)") 0))))roo")
      ->to_string(),
    R"("<string>")");
}

TEST_F(AstReadString, reads_source_string_with_source_path_option)
{
  EXPECT_EQ(
    runtime
      .eval(
        R"roo((:path (:source (nth (roo.ast/read-string "(def x 1)" {:path "inline.roo"}) 0))))roo")
      ->to_string(),
    R"("inline.roo")");
}

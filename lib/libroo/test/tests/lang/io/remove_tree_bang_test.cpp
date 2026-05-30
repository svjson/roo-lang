#include "runtime_fixture.h"
#include <gtest/gtest.h>

using RemoveTreeBang = RooTest::RuntimeTestFixture;

TEST_F(RemoveTreeBang, removes_files_under_path)
{
  fs.add_file("repo/pkg/proof/package.edn", "{}");
  fs.add_file("repo/pkg/proof/src/proof/core.roo", "(ns proof.core)");
  fs.add_file("repo/pkg/other/package.edn", "{}");

  EXPECT_EQ(runtime.eval(R"((roo.io/remove-tree! "repo/pkg/proof"))")->to_string(), "nil");

  EXPECT_FALSE(fs.has_file("repo/pkg/proof/package.edn"));
  EXPECT_FALSE(fs.has_file("repo/pkg/proof/src/proof/core.roo"));
  EXPECT_TRUE(fs.has_file("repo/pkg/other/package.edn"));
}

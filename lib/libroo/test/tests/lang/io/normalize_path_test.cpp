#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NormalizePath = RooTest::RuntimeTestFixture;

TEST_F(NormalizePath, normalizes_a_path_lexically)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/normalize-path "src/./app/../main"))")->to_string(),
            R"("src/main")");
}

TEST_F(NormalizePath, preserves_relative_parent_segments)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/normalize-path "../src/../test"))")->to_string(),
            R"("../test")");
}

TEST_F(NormalizePath, returns_nil_for_nil)
{
  EXPECT_EQ(runtime.eval("(roo.io/normalize-path nil)"), Roo::Constant::NIL);
}

TEST_F(NormalizePath, rejects_non_string_paths)
{
  EXPECT_THROW(runtime.eval("(roo.io/normalize-path :src)"),
               Roo::NoMatchingSignatureException);
}

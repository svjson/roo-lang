#include "runtime_fixture.h"
#include <gtest/gtest.h>

using StringEndsWithPFunction = RooTest::RuntimeTestFixture;

TEST_F(StringEndsWithPFunction, tests_string_suffixes)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/ends-with? "Normal Club" "Club"))"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((roo.string/ends-with? "Normal Club" "Normal"))"),
            *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval(R"((roo.string/ends-with? "Normal Club" ""))"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((roo.string/ends-with? "Club" "Normal Club"))"),
            *Roo::Constant::BOOL_FALSE);
}
